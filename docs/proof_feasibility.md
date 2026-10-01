# 📜 Theoretical Proof: Jackson's Feasibility Theorem

---

## 🎯 1. Theorem Statement

Let $S = \{J_1, J_2, \dots, J_n\}$ be a set of $n$ jobs to be executed non-preemptively on a single machine starting at time $t = 0$. Each job $J_i$ has a processing time $p_i > 0$ and a strict delivery deadline $d_i > 0$.

> **Fundamental Theorem (Jackson's Rule / EDD)**:  
> The set of jobs $S$ can be scheduled such that **100% of jobs are completed on or before their deadlines** ($\forall i, C_i \le d_i$) **if and only if** scheduling the jobs in **Earliest Due Date (EDD)** order ($d_{(1)} \le d_{(2)} \le \dots \le d_{(n)}$) produces zero late jobs.

---

## 🔍 2. Formal Proof by Exchange Argument

### Direction 1: $(\Leftarrow)$ If EDD produces zero late jobs, then $S$ is feasible
This direction is trivial: the EDD schedule is an explicit schedule where every job finishes on time. Thus, a feasible schedule exists.

---

### Direction 2: $(\Rightarrow)$ If $S$ is feasible, then EDD is also feasible

Assume there exists a feasible schedule $\sigma = (J_{\sigma(1)}, J_{\sigma(2)}, \dots, J_{\sigma(n)})$ where all jobs finish on time ($C_{\sigma(k)} \le d_{\sigma(k)}$ for all $k$).

If $\sigma$ is already sorted by Earliest Due Date ($d_{\sigma(1)} \le d_{\sigma(2)} \le \dots \le d_{\sigma(n)}$), the claim holds.

Suppose $\sigma$ is **not** in EDD order. Then, there must exist at least one pair of **adjacent** jobs $J_A$ and $J_B$ such that:
1. $J_A$ is executed immediately before $J_B$ in schedule $\sigma$.
2. Their deadlines violate the EDD order:
$$d_A > d_B$$

Let:
* $t_0$ be the start time of job $J_A$ in schedule $\sigma$.
* The completion time of $J_A$ in $\sigma$ is:
$$C_A = t_0 + p_A$$
* The completion time of $J_B$ in $\sigma$ is:
$$C_B = t_0 + p_A + p_B$$

Since schedule $\sigma$ is feasible by hypothesis:
$$C_A \le d_A \quad \text{and} \quad C_B \le d_B$$

```text
Schedule σ:       |--- J_A ---|--- J_B ---|
Time:            t0          C_A         C_B ≤ d_B < d_A

Schedule σ':      |--- J_B ---|--- J_A ---|
Time:            t0          C'_B        C'_A = C_B
```

Now, consider a new schedule $\sigma'$ obtained by **swapping only $J_A$ and $J_B$**, keeping all other jobs unchanged:
1. In $\sigma'$, $J_B$ runs first from $t_0$ to $C'_B = t_0 + p_B$.
2. In $\sigma'$, $J_A$ runs second from $C'_B$ to $C'_A = t_0 + p_B + p_A = C_B$.

Let us inspect the feasibility of every job in $\sigma'$:
* **For jobs preceding $J_A$ and $J_B$**: Their start and completion times are completely unchanged. They remain on time.
* **For jobs following $J_A$ and $J_B$**: Since $C'_A = C_B$, the total time occupied by $\{J_A, J_B\}$ is unchanged ($p_A + p_B$). Therefore, all subsequent jobs start and complete at the exact same times as in $\sigma$. They remain on time.
* **For job $J_B$**:
  $$C'_B = t_0 + p_B < t_0 + p_A + p_B = C_B \le d_B$$
  Thus, $C'_B < d_B$, meaning $J_B$ finishes strictly earlier and is still on time!
* **For job $J_A$**:
  $$C'_A = C_B \le d_B$$
  Since $d_B < d_A$, we have:
  $$C'_A \le d_B < d_A \implies C'_A < d_A$$
  Thus, $J_A$ is also strictly on time!

### Conclusion of the Swap:
Swapping an adjacent inverted pair $(J_A, J_B)$ strictly reduces the number of deadline inversions by 1 **without causing any job to be late**.

By repeatedly applying this adjacent exchange (identical to the Bubble Sort termination argument), we can eliminate all inversions in a finite number of steps (at most $\frac{n(n-1)}{2}$ swaps) until the schedule becomes the EDD schedule $\sigma_{\text{EDD}}$.

Because no swap ever creates a late job, the resulting **EDD schedule is guaranteed to be 100% on time**. $\blacksquare$

---

## 🔑 3. Fundamental Property: Independence from Penalties $w$

> **Key Takeaway**:  
> Whether a set of jobs can be delivered without delay depends **strictly on processing times $p$ and deadlines $d$**.  
> The penalties $w$ play **zero role** in feasibility. A job is either on time or late; how much money it costs when late cannot physically change the machine's timeline.

This property is the foundation for:
1. The **5-line Feasibility Judge** (used as the ultimate oracle for all subsets).
2. The **Theoretical $k$-Penalty Upper Bound** (Milestone 4): no algorithm can ever save more than $k = |S_{\text{max}}|$, where $k$ is the maximum on-time set size regardless of weights.
