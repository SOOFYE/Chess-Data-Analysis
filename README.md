# Chess Data Analysis
This project aims to analyze a dataset of 6.25 million chess games played on lichess.org to provide insights into player statistics and performance. The program extracts necessary data from each column and derives conclusions about various aspects of a chess game.

# Introduction
The opening of a chess game is crucial, and the type of opening can determine the direction the game will take. The program analyzes which players with certain ELO ratings prefer to use specific openings and the probability of winning while using the opening. The program also considers the specific events used in each game as every event duration differs from one another. While extracting data, the program assumes that the opening referred to is always for the player playing as White.

# Background
The research for this project began with a search for a suitable dataset of at least 1GB for analysis. Opinions were sought from different teachers and senior students, and all suggestions were incorporated into the program using the data structures learned during the course, relevant materials, and some out-of-the-box ideas to make an effective and efficient program.

# Problem Analysis
The project's goal was to extract valuable and reasonable meanings from the large dataset provided. With a small-scale dataset, subtle population patterns and heterogeneities are not possible, but with a large dataset, the program can extract meaningful conclusions.

# Implementation
The program begins by reading a 1.6GB CSV file, which was reduced from 4GB using scraped data using Python's Pandas Library. The CSV file is read using 'vincentlaucsb csv-parser', which reads each data from the column and saves the data in the program's object 'rowobjects' vector of class RowData. After saving the data, the program maps all user data using the unordered_map data structure. The user-ID, represented by White and Black columns, is used as the key, and the value mapped is the user's statistics, such as wins, losses, draws, most frequent events attended, and the most frequent opening played in all their games.

The program uses the unordered_map data structure because it is built on the implementation of hash tables, making it easy to map keys to their statistics. It also provides quick searching and insertion times, where the average case is O(1) and the worst is O(n), where n is the number of elements.

To make searching for users more efficient, the program implemented the Trie data structure. The Trie allows retrieval of information in the most effective and efficient way. If the user inputs a username that does not match the program's data, the program gives suggested usernames that the user might have intended to search. Using Trie, search complexities can be brought to the optimal limit (key length).

The program's modules also determine the number of times a type of opening is used with a set ELO range. As ELO tells us about the player's rank, knowing the frequency of the type of opening used in a given ELO range can give insight into the type of strategy players with similar ELO prefer over others. The program uses the implementation of priority_queue, which arranges each opening according to its usage, with the most frequent opening on the top of the queue.

# Conclusion
The program provides statistical and analytical calculations through which a conclusion can be drawn about the dataset analyzed. The program's use of various data structures provides efficient and effective ways to retrieve information, making the program an excellent tool for analyzing large datasets.

## Relationship b/w White players ELO VS white players Rating
![image](https://user-images.githubusercontent.com/84918625/225044710-dfdf6dfd-a729-4b91-8292-65fa258789ca.png)
