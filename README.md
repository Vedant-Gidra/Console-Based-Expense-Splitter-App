# Console-Based Expense Splitter App

This is a simple **C++ console application** designed to manage and split expenses among members of a group. The app supports creating groups, recording payments, settling balances, and persisting group data to a text file (`Database.txt`).

---

## Features

✅ Create and manage multiple expense groups  
✅ Add new payments to existing groups  
✅ Settle payments between group members  
✅ View complete details of all existing groups  
✅ Delete groups from the database  
✅ Persist group data between sessions (stored in `Database.txt`)

---

## How It Works

1. **Group Creation**  
   - Enter a unique group name.
   - Add members to the group.
   - Record payment transactions made by members.
   - The app calculates how much each member should pay or receive to settle evenly.

2. **Existing Group Operations**  
   - Add new payments for existing groups.
   - Settle payments (adjust balances between group members).
   - Delete any group if no longer required.
   - View the current status of each group.

3. **Data Storage**  
   - All group information is appended to a `Database.txt` file.
   - The app reads from this file during startup to retrieve existing groups.

---
## How to Compile and Run

### Prerequisites
- A C++ compiler (`g++` recommended)

### Compile
```bash
g++ split.cpp -o split
```

### Run
```bash
./split
```

---

## Functionalities Explained

| Functionality | Description |
|---------------|-------------|
| `newGroup()` | Creates a new expense group, records member names and payments. |
| `existingGroup()` | Menu for updating payments, settling debts, or deleting a group. |
| `ViewExistingGroup()` | Displays all existing groups with payment details. |
| `DividePayment()` | Core logic to calculate who pays or receives how much. |
| `writeInFile()` | Saves the group data to `Database.txt`. |
| `retrievePersonInfo()` | Reads data of a specific group from the database file. |

---

## Example Flow

1. Create a group called `trip2025`  
2. Add members: Alice, Bob, Charlie  
3. Add payments:  
   - Alice paid 300  
   - Bob paid 150  
   - Charlie paid 50  
4. The app calculates individual shares and who owes whom.  
5. View and settle payments over time.

---

## Database File Format (`Database.txt`)

```
Group name: trip2025
name: alice
amtPaid: 300
amtToPay: 0
amtToReceive: 150
name: bob
amtPaid: 150
amtToPay: 50
amtToReceive: 0
name: charlie
amtPaid: 50
amtToPay: 100
amtToReceive: 0
**
```

---

## Known Limitations

- Console-only interface (no GUI).
- Assumes equal distribution of total expenses.
- No error handling for file read/write failures beyond basic checks.

---

## Future Improvements

- Add a user-friendly GUI.
- Support weighted shares (unequal splits).
- Add transaction history and reports.
- Encrypt sensitive data.

---

## Author

- **Vedant Gidra**

---
