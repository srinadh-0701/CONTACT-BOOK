# Contact Book (Linked List + File Persistence)

**Data Structures:** Singly **linked list** (self-referential struct) with **alphabetical sorted insertion**.  
**Persistence:** Contacts saved/loaded from `contacts.csv` automatically.

## Features
- Add, list, search (by name or phone), update, delete.
- Sorted by **name** (case-insensitive).
- Autosave to CSV on exit; autoload on start.

## Build & Run
```bash
g++ -std=c++17 -O2 -o ContactBook main.cpp
./ContactBook
```

**CSV format:** `name,phone,email` (commas inside fields not supported for simplicity).
