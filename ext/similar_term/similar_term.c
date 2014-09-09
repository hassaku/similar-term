//
// Originally, most of logics in this source code is :
//
//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//  repository: http://word2vec.googlecode.com/svn/trunk/distance.c
//

#include <ruby.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

const long long max_size = 1000;         // max length of strings
const long long N = 10;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries
float *M;
long long words, size;
char *vocab;

static VALUE
load(VALUE self, VALUE file_name)
{
  FILE *fp;
  float len;
  long long a, b;
  char ch;

  fp = fopen(StringValuePtr(file_name), "rb");
  if (fp == NULL) {
    printf("Input file not found\n");
    return Qfalse;
  }

  fscanf(fp, "%lld", &words);
  fscanf(fp, "%lld", &size);
  vocab = (char *)malloc((long long)words * max_w * sizeof(char));

  M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
    fclose(fp);
    return Qfalse;
  }

  for (b = 0; b < words; b++) {
    fscanf(fp, "%s%c", &vocab[b * max_w], &ch);
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, fp);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++) M[a + b * size] /= len;
    if(b % 1000 == 0) {
      printf("%lld / %lld\r", b, words);
    }
  }
  printf("\n");
  fclose(fp);

  return Qtrue;
}

static VALUE
similar_terms(VALUE self, VALUE rb_keyword)
{
  float dist, bestd[N], vec[max_size];
  char *bestw[N];
  float len;
  char st[100][max_size];
  long long a, b, c, d, cn, bi[100];

  char *keyword = StringValuePtr(rb_keyword);
  VALUE ary = rb_ary_new();

  for (a = 0; a < N; a++) bestw[a] = (char *)malloc(max_size * sizeof(char));
  for (a = 0; a < N; a++) bestd[a] = 0;
  for (a = 0; a < N; a++) bestw[a][0] = 0;
  cn = 0;
  b = 0;
  c = 0;

  while (1) {
    st[cn][b] = keyword[c];
    b++;
    c++;
    st[cn][b] = 0;
    if (keyword[c] == 0) break;
    if (keyword[c] == ' ') {
      cn++;
      b = 0;
      c++;
    }
  }

  cn++;
  for (a = 0; a < cn; a++) {
    for (b = 0; b < words; b++) {
      if (!strcmp(&vocab[b * max_w], st[a])) break;
    }
    if (b == words) b = -1;
    bi[a] = b;
    if (b == -1) {
      printf("Out of dictionary word!\n");
      break;
    }
  }
  if (b == -1) return Qnil;

  for (a = 0; a < size; a++) vec[a] = 0;
  for (b = 0; b < cn; b++) {
    if (bi[b] == -1) continue;
    for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
  }
  len = 0;
  for (a = 0; a < size; a++) len += vec[a] * vec[a];
  len = sqrt(len);
  for (a = 0; a < size; a++) vec[a] /= len;
  for (a = 0; a < N; a++) bestd[a] = -1;
  for (a = 0; a < N; a++) bestw[a][0] = 0;
  for (c = 0; c < words; c++) {
    a = 0;
    for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
    if (a == 1) continue;
    dist = 0;
    for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
    for (a = 0; a < N; a++) {
      if (dist > bestd[a]) {
        for (d = N - 1; d > a; d--) {
          bestd[d] = bestd[d - 1];
          strcpy(bestw[d], bestw[d - 1]);
        }
        bestd[a] = dist;
        strcpy(bestw[a], &vocab[c * max_w]);
        break;
      }
    }
  }
  for (a = 0; a < N; a++) {
    rb_ary_push(ary, rb_str_new2(bestw[a]));
  }

  return ary;
}

void Init_similar_term()
{
  VALUE class;
  class = rb_define_class("SimilarTerm", rb_cObject);
  rb_define_method(class, "load", load, 1);
  rb_define_method(class, "similar_terms", similar_terms, 1);
}
