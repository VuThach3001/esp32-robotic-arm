# Cornell Notes

## Topic: Markdown Hints when using Visual Studio Code

## Date: 17/03/2026

---

### Cue Column (Questions, Keywords, or Prompts)

- Why don't snippets work in Markdown files?
- How to enable quick suggestions in Markdown?
- How to create custom workspace snippets?
- What is the correct snippet file format?

---

### Notes Section (Main Notes)

- **VS Code disables quick suggestions in Markdown by default** to avoid interfering with writing. This is why snippets trigger in `.txt` but not in `.md` files.
- To enable snippets in Markdown, add the following to `.vscode/settings.json`:

  ```json
  "[markdown]": {
      "editor.quickSuggestions": {
          "other": "on",
          "comments": "off",
          "strings": "off"
      },
      "editor.snippetSuggestions": "top"
  }
  ```

- **Workspace snippets** must use the `.code-snippets` extension (e.g., `.vscode/cornell-notes.code-snippets`). A plain `.json` file in `.vscode/` will **not** be recognized as a snippet file.
- Snippet tab stops (`$1`, `$2`, etc.) allow you to jump between fields with Tab. `$0` defines the final cursor position.
- Placeholders with defaults use the syntax `${1:default text}`.

---

### Summary Section (Summary of Notes)

VS Code disables quick suggestions in Markdown by default. To use custom snippets in `.md` files, enable `editor.quickSuggestions` for the `[markdown]` language scope in settings. Workspace snippets must be saved as `.code-snippets` files inside the `.vscode/` folder.
