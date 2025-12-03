# Card Game using Data Structures (C)

An advanced card-drawing game implemented in pure C, demonstrating practical use of multiple data structures and algorithms within a single project.

Players draw cards from a shuffled 52-card deck, compete over several rounds, and the system ranks them using a Binary Search Tree based on their final score.

This project is ideal for university assignments, data-structures courses, or portfolio showcasing.

✨ Features
🃏 Full 52-card deck

Represents each card using (value, suit)

Automatically generated and Fisher–Yates shuffled

📦 Stack (LIFO) — Deck implementation

Cards are stored in a stack

Players draw cards using pop()

📥 Queue — Cards played each round

Each round stores played cards in a queue

Used to determine the round winner

🔁 Circular Queue — Player turn rotation

Automatically cycles through players (2–4)

Eliminates manual index handling

📄 Linked List — Round history

Stores:

round number

winner name

winner score after the round

Printed at the end of the game

🌳 Binary Search Tree (BST) — Final ranking

Inserts each player by score

In-order traversal prints ranking from lowest → highest

🏅 Scoring

Winner of each round earns +100 points

Tie = no points awarded

🎮 Game Rules

Choose 2–4 players

Each player enters their name

Deck gets created, shuffled, and loaded into the stack

Game plays 5 rounds (or fewer if deck runs out)

Each round:

Every player draws one card

Highest value wins (+100 points)

After all rounds:

Round history prints from the linked list

Final ranking prints using BST ordering

🛠️ Compilation

Make sure GCC is installed.
```bash
gcc card_game_with_data_structures.c -o card_game
```

Run:
```bash
./card_game
```

📁 File Structure
```bash
card-game-ds/
│── card_game_with_data_structures.c
│── README.md
```

🧠 Concepts Demonstrated

- Stack (LIFO)
- Queue (FIFO)
- Circular Queue
- Linked List
- Binary Search Tree (BST)
- Structs in C
- Dynamic memory management (malloc, free)
- Fisher–Yates shuffle algorithm
- Modular game logic
- Multi-structure interaction in one program

This project highlights multi-structure orchestration — perfect for demonstrating strong understanding of data structures in real applications.
