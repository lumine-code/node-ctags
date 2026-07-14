"use strict";

const path = require("node:path");
const ctags = require("../lib/ctags");

const tagsFile = path.join(__dirname, "fixtures", "tags");

describe("ctags", () => {
  it("findTags returns matching tags", async () => {
    const tags = await new Promise((resolve, reject) => {
      ctags.findTags(tagsFile, "duplicate", (error, matches) => {
        if (error) reject(error);
        else resolve(matches);
      });
    });

    expect(tags.length).toBe(2);
    expect(tags[0].name).toBe("duplicate");
    expect(tags[1].name).toBe("duplicate");
  });

  it("createReadStream emits tags in chunks", async () => {
    const chunks = [];
    for await (const chunk of ctags.createReadStream(tagsFile, { chunkSize: 3 })) {
      chunks.push(chunk);
    }

    expect(chunks.map((chunk) => chunk.length)).toEqual([3, 1]);
    expect(chunks.flat().length).toBe(4);
  });

  it("createReadStream reports missing files", async () => {
    const missingFile = path.join(__dirname, "fixtures", "missing-tags");
    const consumeMissingFile = async () => {
      for await (const _ of ctags.createReadStream(missingFile)) {
        // Consume the stream.
      }
    };

    await expectAsync(consumeMissingFile()).toBeRejectedWithError(/Tags file could not be opened/);
  });
});
