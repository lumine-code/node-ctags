"use strict";

const assert = require("node:assert/strict");
const path = require("node:path");
const test = require("node:test");
const ctags = require("../lib/ctags");

const tagsFile = path.join(__dirname, "fixtures", "tags");

test("findTags returns matching tags", async () => {
  const tags = await new Promise((resolve, reject) => {
    ctags.findTags(tagsFile, "duplicate", (error, matches) => {
      if (error) reject(error);
      else resolve(matches);
    });
  });

  assert.equal(tags.length, 2);
  assert.equal(tags[0].name, "duplicate");
  assert.equal(tags[1].name, "duplicate");
});

test("createReadStream emits tags in chunks", async () => {
  const chunks = [];
  for await (const chunk of ctags.createReadStream(tagsFile, { chunkSize: 3 })) {
    chunks.push(chunk);
  }

  assert.deepEqual(chunks.map((chunk) => chunk.length), [3, 1]);
  assert.equal(chunks.flat().length, 4);
});

test("createReadStream reports missing files", async () => {
  const missingFile = path.join(__dirname, "fixtures", "missing-tags");
  await assert.rejects(async () => {
    for await (const _ of ctags.createReadStream(missingFile)) {
      // Consume the stream.
    }
  }, /Tags file could not be opened/);
});
