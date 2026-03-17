# Cornell Notes

## Topic: Git Submodules and Cloning with Submodules

## Date: 17/03/2026

---

### Cue Column (Questions, Keywords, or Prompts)

- How to clone with submodules?
- What if I forgot to use `--recurse-submodules`?
- How to update submodules after pulling?
- How to check submodule status?
- What is detached HEAD in submodules?
- How to run commands on all submodules?

---

### Notes Section (Main Notes)

**1. Cloning the repository (with submodules)**

This project uses Git submodules. To avoid missing content, clone with submodules in a single step:

```bash
git clone --recurse-submodules https://github.com/TNH510/NeoVerse.git
```

Or (PowerShell on Windows – same command):

```powershell
git clone --recurse-submodules https://github.com/TNH510/NeoVerse.git
```

Then enter the directory:

```bash
cd NeoVerse
```

**If you already cloned without `--recurse-submodules`**

Run:

```bash
git submodule update --init --recursive
```

**2. Updating after pulling new changes**

When you pull new commits that move submodule pointers, include submodules:

```bash
git pull --recurse-submodules
git submodule update --init --recursive  # (usually redundant after the pull, but safe)
```

If you want Git to always recurse automatically for this repo:

```bash
git config submodule.recurse true
```

Globally (applies to all repos):

```bash
git config --global submodule.recurse true
```

**3. Refresh submodules to the latest remote commits (only if you intentionally track remote heads)**

Normally you should stay on the exact commit recorded by the superproject. If you intentionally want the latest remote changes inside each submodule branch:

```bash
git submodule update --remote --merge
```

**4. Shallow clone (faster) with submodules**

```bash
git clone --depth 1 --recurse-submodules --shallow-submodules https://github.com/TNH510/NeoVerse.git
```

**5. Check submodule status**

View the current commit each submodule is on:

```bash
git submodule status
```

For more detailed information:

```bash
git submodule foreach git status
```

**6. Execute commands on all submodules**

Run any Git command on all submodules at once:

```bash
git submodule foreach 'git fetch'
git submodule foreach 'git checkout main'
```

**7. Sync submodule URLs**

If submodule URLs change in `.gitmodules`, sync them:

```bash
git submodule sync --recursive
git submodule update --init --recursive
```

**8. Understanding detached HEAD state**

Submodules are typically in "detached HEAD" state (pointing to a specific commit, not a branch). This is normal. To work on a branch inside a submodule:

```bash
cd path/to/submodule
git checkout main  # or any branch
# make changes, commit, push
cd ..
git add path/to/submodule
git commit -m "Update submodule to latest"
```

### Common issues

* Forgot `--recurse-submodules`: submodule folders exist but are empty -> run the init/update command above.
* Authentication errors (private submodules): ensure you have access and are logged in (Git Credential Manager or a PAT). Re-run `git submodule update --init --recursive` after fixing credentials.
* Local edits inside a submodule not showing in main repo commits: commit & push inside the submodule first, then commit the updated submodule pointer in the main repo.

---

### Summary Section (Summary of Notes)

Git submodules allow embedding external repositories within a parent repository. Always clone with `--recurse-submodules` or run `git submodule update --init --recursive` afterward. Use `git pull --recurse-submodules` or set `submodule.recurse true` for automatic updates. Submodules stay in detached HEAD state by default, pointing to specific commits. Use `git submodule status` to check current state and `git submodule foreach` to run batch commands. Remember to commit changes inside the submodule first, then update the submodule pointer in the parent repo.
