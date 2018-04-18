# Tic-Tac-Toe
I have created an automated player that can play with humans. It computes probabilities of winning for every possible move using the technic called backtracking. Then, 
if an opponent does have two in a row, bot blocks the opponent. Otherwise, bot chooses the move with highest possibility of winning.

To improve the time of computation, I have utilized multithreading. I have designed a pool of threads for this project to improve performance. That has provided 2x speedup.
