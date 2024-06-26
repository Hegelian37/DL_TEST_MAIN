To run the code, you will have to go the path of Bitcoin\x64\Debug.
There you will see files Bitcoin.exe, launch.bat, and transactions.csv.
To launch the program, one should double-click the launch.bat.
Output produced by the program in the command line is dependent on the input, which is the transactions.csv. For simplicity, I've created a file with 5000 rows of information - but you are free to use a file with practically unlimited number of rows. Just remember that in .csv files, columns in the rows are separated by commas or semicolons.
The program will go through this file, seeking for the transactions with bigger fees relative to the amount, composing a Block weighting no more than 1 MB by using a greedy sorting algorithm. For 5000 to 10000 rows, according to my tests, it takes no more than 0.13 to 0.20 seconds. This result certainly can be optimized further, but there is no practical need in this.
