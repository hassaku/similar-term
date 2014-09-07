# SimilarTerm

List similar terms by word2vec.

## Installation

Add this line to your application's Gemfile:

    gem 'similar-term'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install similar-term

## Usage

```
st = SimilarTerm.new
# Load pre-trained word and phrase vectors (see https://code.google.com/p/word2vec)
st.load("/path/to/pre_trained.bin")
st.similar_terms("penguin")
# => ["penguins", "otter", ...]
```

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
