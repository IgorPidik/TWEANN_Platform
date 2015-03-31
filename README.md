Topology and Weight evolving artificial neural networks
=======
This is my open-source project focused on **machine learning** and **artificial intelligence**.
This API can be used to train(evolve) **feed-forward** and **recurrent** artificial neural networks.
## How it works
Platform generates N neural networks with set or random topology and with random weights. You have to choose between
**recurrent** and **feed-forward** type of neural network. Then platform scores every network, sorts them and delete
all neural networks but 3 top-scored. Afterwards platform replace deleted neural networks with copies of top neural network
and mutates them. This process continues until max possible attempts are used or requested score(fitness) is reached.

## Overview
* **ExampleController.h,.cpp** and **ExampleScape.h,cpp** shows full example how can this API be used. And should be deleted in your project.
* **NeuralNetwork.h,.cpp** - Class NeuralNetwork represent actual neural network with methods for **proccessing input**, coping, mutating.
* **Neuron.h,.cpp** - Basic unit of neural networks handles input from other neurons, proccessing and sending value to other neurons.
* **Connections.h,.cpp** - Handles adding, storing, deleting, updating... of weights between neurons.
* **IScapeInterface.h** - Interface for scape class, which provides inputs for training and scores outputs from neural networks.
