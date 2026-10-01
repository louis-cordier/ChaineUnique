# 🎓 Project Presentation Notes — Single-Machine Job Scheduling ("Chaîne Unique")

---

## 🎙️ 1. The 30-Second Elevator Pitch
> *"In our industrial printing workshop, we have **one single production line** that cannot do two jobs at once and cannot be interrupted once a job has started.  
> Right now, the workshop supervisor schedules orders naively by earliest deadline (EDD). When a very long job runs first, it occupies the machine and cascades delays onto all subsequent jobs.  
> Because our contracts follow an **All-or-Nothing** rule (1 minute late = 100% of the penalty due), the company is losing significant revenue.  
> My mission is to scientifically compare 4 scheduling algorithms (R1, R2, R3, R3') in C++17, measure the actual money saved, verify our algorithms with mathematical oracles, and deliver an actionable 1-page executive memo recommending the best strategy for Monday morning."*

---

## 🛠️ 2. What Has Been Implemented (Milestone 0)

| Component | File | What It Does | Why It Matters |
| :--- | :--- | :--- | :--- |
| **Data Model** | [`src/order.hpp`](file:///c:/Users/gamer/Documents/Bachelor%20IT/B3/Temps%20Plein/ChaineUnique/src/order.hpp) | Stores processing time $p$, deadline $d$, penalty $w$, ratio $w/p$, and `original_line`. | Preserves original line index for the strict tie-breaking rule (largest $p$, then lowest in source file). |
| **Order Parser** | [`src/order_book_parser.cpp`](file:///c:/Users/gamer/Documents/Bachelor%20IT/B3/Temps%20Plein/ChaineUnique/src/order_book_parser.cpp) | Reads `.txt` files (`ID p d w`), ignores comments (`#`) and empty lines, validates positive integers. | Ensures input reliability without external dependencies. |
| **Build System** | [`CMakeLists.txt`](file:///c:/Users/gamer/Documents/Bachelor%20IT/B3/Temps%20Plein/ChaineUnique/CMakeLists.txt) & [`Makefile`](file:///c:/Users/gamer/Documents/Bachelor%20IT/B3/Temps%20Plein/ChaineUnique/Makefile) | Configured for **modern C++17** with `-O3` compiler optimizations. | Ultra-fast execution for massive benchmarks (2,000 orders) and exact combinatorial search ($N=18$). |
| **Git Architecture** | Git repository | Structured with `main` (stable), `develop` (integration), and feature branches. | Professional, auditable engineering process with one branch per milestone. |

---

## 🗺️ 3. Full Project Roadmap

```text
[✅ Milestone 0] Foundation & File Parser (DONE)
      ↓
[⏳ Milestone 1] Deterministic Generator & ASCII Timeline (IN PROGRESS)
      ↓
[   Milestone 2] Feasibility Judge (Earliest Due Date) & Exchange Proof
      ↓
[   Milestone 3] The 4 Scheduling Rules (R1, R2 Moore-Hodgson, R3, R3')
      ↓
[   Milestone 4] Theoretical Bound & Exact Combinatorial Verifier (N ≤ 18)
      ↓
[   Milestone 5] Automated Suite for the 5 Oracles
      ↓
[   Milestone 6] Massive Benchmark Campaign (Up to 2,000 orders / CSV)
      ↓
[   Milestone 7] Adversarial Trap Cases & 1-Page Executive Memo
```

---

## 🎯 4. Current Work: Milestone 1 (Order Generator & ASCII Frise)
- **Custom 32-bit PRNG**: Implementing Xorshift32 for deterministic random numbers.
- **Solution Planting**: Creating a set of orders guaranteed to finish on time, saving its size in an external witness file (`witness_*.txt`) that our scheduling algorithms will never see.
- **Noise Injection**: Adding parasite orders with tight deadlines to saturate machine capacity.
- **ASCII Frise**: Visual 1-char-per-unit terminal timeline display for instances with cumulative duration $\le 60$.
