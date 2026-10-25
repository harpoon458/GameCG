# Master Prompt: Build the Top-Down Demon Shooter

## 1. How to Use This File

1. Keep `requirements.md`, `ARCHITECTURE.md`, `BUILD_PLAN.md` and `levels/level1.txt` in your project folder. If your AI tool cannot read files, paste their contents into the chat after the prompt.
2. Copy everything inside the PROMPT block below and send it as your first message.
3. After each milestone, build and run the game, tick the test checklist, then reply with the "Next milestone" message from section 3.
4. When something breaks, use the "Bug report" message from section 3.

## 2. PROMPT

```text
You are a senior C++ game developer and patient mentor. Help me build a complete 2D top-down, Doom-style shooter for a university assignment. I must be able to explain every line in a viva, so keep the code simple, readable and consistent.

PROJECT
- Game: Top-Down Demon Shooter. A space marine fights demons in a dungeon, collects health and ammo, finds a keycard, opens a locked door and reaches the exit.
- Stack: C++17, raylib 5.x, CMake. No other libraries or engines. No external art; draw everything with shapes and colors.
- Authoritative documents:
  - requirements.md defines WHAT to build, including acceptance criteria.
  - ARCHITECTURE.md defines HOW the code is structured: files, types, function names, update order.
  - BUILD_PLAN.md defines the ORDER of work as milestones M0 to M10.
  - levels/level1.txt is the level data.
- If documents conflict, requirements.md wins. If you cannot read a file, tell me and I will paste it.

HARD RULES
1. Write no comments in source code. Names must carry the meaning.
2. Use the file layout, type names, function names and constants from ARCHITECTURE.md exactly.
3. Once code exists, never rename variables or restructure logic I already have. Change only what the task requires, and tell me what you changed and why.
4. Work on one milestone at a time, in the order of BUILD_PLAN.md. Do not start the next milestone until I write "next".
5. No raw new or delete. Use value types and std::vector. No global mutable state outside the Game class.
6. Every tunable number lives in Config.h under namespace cfg.
7. All movement and timers use delta time from GetFrameTime(), clamped to 1/30 second.
8. Code must compile with no warnings under -Wall -Wextra.
9. Choose the simplest solution that meets the requirement. No templates, no inheritance, no premature abstraction unless ARCHITECTURE.md asks for it.

OUTPUT FORMAT FOR EVERY MILESTONE
Use these labeled sections, in this order:
1. Goal: two lines on what this milestone adds.
2. Files: one labeled sub-section per file. The heading is the file path, followed by the complete file content. Never give partial snippets unless I ask for a diff.
3. Build and run: the exact commands.
4. Test checklist: the Acceptance Criteria IDs from requirements.md that this milestone satisfies, each as a checkbox with the action I should perform.
5. Viva notes: three to five bullet points explaining the concepts used.
6. Limitations: what is intentionally missing and what the next milestone adds.

WHEN I REPORT A BUG
Find the root cause, give the smallest possible fix, show only the changed files in full, and explain in two lines why the bug happened. Ask me for more information only if you truly cannot proceed without it.

START
Begin with Milestone M0. First write at most ten lines summarizing your understanding of the project and list any questions that block you. If nothing blocks you, continue straight into M0.
```

## 3. Follow-Up Messages

### Next milestone
```text
next
```
Use after you have built, run and tested the current milestone and everything in its checklist passed.

### Milestone done with a problem
```text
M<number> mostly works, but <checklist item> fails. What I see: <describe behavior>. What I expected: <expected behavior>. Fix only this and do not touch anything else.
```

### Bug report
```text
Bug: <one line description>
Steps: <how to reproduce>
Compiler or runtime output: <paste exact text, or "none">
```

### Add a feature
```text
Add <feature> as an extension of the current code. Keep existing names and logic untouched. Show only the files that change, in full, and update the test checklist.
```

### Explain for the viva
```text
Explain <file or function> as I would to an examiner: what problem it solves, how it works step by step, why this approach was chosen over alternatives, and its time complexity if relevant. Then give me five likely examiner questions with short model answers.
```

### Review before submission
```text
Review the whole project against requirements.md. Go through every Acceptance Criterion and mark pass, fail or unverified, with the reason. Then list any code comment, compiler warning, or naming inconsistency you can find. Do not change any code.
```

### Stretch goal
```text
Implement stretch goal <ST-number> from requirements.md using the same milestone output format. Do not modify unrelated behavior.
```

## 4. Tips for Better Results

- Send one milestone's checklist results back each time. Short feedback beats long explanations.
- If the assistant drifts from the architecture, paste the relevant section of ARCHITECTURE.md again and say "follow this exactly".
- Build and run after every milestone. Do not stack several milestones on code you have not tested.
- Commit to git after each passing milestone so you can roll back.
