"use strict";

const { Readable } = require("stream");
const { Tags } = require("../build/Release/ctags.node");

exports.findTags = function findTags(tagsFilePath, tag, options, callback) {
  if (typeof tagsFilePath !== "string") {
    throw new TypeError("tagsFilePath must be a string");
  }
  if (typeof tag !== "string") {
    throw new TypeError("tag must be a string");
  }
  if (typeof options === "function") {
    callback = options;
    options = null;
  }

  const { partialMatch, caseInsensitive } = options ?? {};
  const tagsWrapper = new Tags(tagsFilePath);
  tagsWrapper.findTags(tag, partialMatch, caseInsensitive, (error, tags) => {
    tagsWrapper.end();
    callback?.(error, tags);
  });
};

exports.createReadStream = function createReadStream(tagsFilePath, options = {}) {
  if (typeof tagsFilePath !== "string") {
    throw new TypeError("tagsFilePath must be a string");
  }

  const chunkSize = typeof options.chunkSize === "number" ? options.chunkSize : 100;
  const tagsWrapper = new Tags(tagsFilePath);
  let reading = false;

  return new Readable({
    objectMode: true,
    read() {
      if (reading) return;
      if (!tagsWrapper.exists()) {
        this.destroy(new Error(`Tags file could not be opened: ${tagsFilePath}`));
        return;
      }

      reading = true;
      tagsWrapper.getTags(chunkSize, (error, tags) => {
        reading = false;
        if (error) {
          tagsWrapper.end();
          this.destroy(error);
        } else if (tags.length === 0) {
          tagsWrapper.end();
          this.push(null);
        } else if (this.push(tags)) {
          this.read(0);
        }
      });
    },
  });
};
