# @lumine-code/ctags

Reads and streams source code tag files from Node.js.

## Features

- **Tag lookup**: finds exact, partial, and case-insensitive matches in ctags files.
- **Streaming reads**: emits parsed tags in chunks without loading the full file into memory.
- **Node-API addon**: provides native tag parsing without relying on V8-specific APIs.

## Installation

```sh
npm install @lumine-code/ctags
```

## Building

```sh
npm ci
npm run build
npm test
```

## Documentation

### findTags(tagsFilePath, tag, [options], callback)

Get all tags matching the tag specified from the tags file at the path.

* `tagsFilePath` - The string path to the tags file.

* `tag` - The string name of the tag to search for.

* `options` - An optional options object containing the following keys:

  * `caseInsensitive` - `true` to include tags that match case insensitively,
    (default: `false`)
  * `partialMatch` - `true` to include tags that partially match the given tag
    (default: `false`)

* `callback` - The function to call when complete with an error as the first
             argument and an array containing objects that have `name` and
             `file` keys and optionally a `pattern` key if the tag file
             specified contains tag patterns.

#### Example

```js
const ctags = require('@lumine-code/ctags')

ctags.findTags('/Users/me/repos/node/tags', 'exists', (error, tags = []) => {
  if (error) throw error
  for (const tag of tags) console.log(`${tag.name} is in ${tag.file}`)
})
```

### createReadStream(tagsFilePath, [options])

Create a read stream to a tags file.

The stream returned will emit `data` events with arrays of tag objects
that have `name` and `file` keys and optionally a `pattern` key if the tag file
specified contains tag patterns.

An `error` event will be emitted if the tag file cannot be read.

An `end` event will be emitted when all the tags have been read.

* `tagsFilePath` - The string path to the tags file.

* `options` - An optional object containing the following keys.

  * `chunkSize` - The number of tags to read at a time (default: `100`).

Returns a stream.
#### Example

```js
const ctags = require('@lumine-code/ctags')

const stream = ctags.createReadStream('/Users/me/repos/node/tags')
stream.on('data', tags => {
  for (const tag of tags) {
    console.log(`${tag.name} is in ${tag.file} with pattern: ${tag.pattern}`)
  }
})
```

## Contributing

Got ideas to make this package better, found a bug, or want to help add new features? Just drop your thoughts on GitHub. Any feedback is welcome!
