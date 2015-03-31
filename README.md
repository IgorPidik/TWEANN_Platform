#Topology and Weight evolving artificial neural networks
===============
This is my open-source project focused on **machine learning** and **artificial intelligence**.
You can use this tool to train(evolve) **feed-forward** and **recurrent** artificial neural networks.

## How it works
Platform generates N neural networks with set or random topology and with random weights. You have to choose between
**recurrent** and **feed-forward** type of neural network. Then platform scores every network, sorts them by it and delete
all neural networks but 3 top-scored. Afterwards platform replace deleted neural networks with copies of top neural network
and mutates them. This process continues until max possible attempts are used or requested score(fitness) is reached.
