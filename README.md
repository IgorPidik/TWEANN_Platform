Topology and Weight evolving artificial neural networks
=======
This is my open-source project focused on **machine learning** and **artificial intelligence**.
This tool can be used to train(evolve) **feed-forward** and **recurrent** artificial neural networks.
##How it works
Platform generates N neural networks with set or random topology and with random weights. You have to choose between
**recurrent** and **feed-forward** type of neural network. Then platform scores every network, sorts them and delete
all neural networks but 3 top-scored. Afterwards platform replace deleted neural networks with copies of top neural network
and mutates them. This process continues until max possible attempts are used or requested score(fitness) is reached.

##Overview
* **ExampleController.h,.cpp** and **ExampleScape.h,cpp** shows full example how can this API be used. And should be deleted in your project.
* **NeuralNetwork.h,.cpp** - Class NeuralNetwork represent actual neural network with methods for **proccessing input**, coping, mutating.
* **Neuron.h,.cpp** - Basic unit of neural networks handles input from other neurons, proccessing and sending value to other neurons.
* **Connections.h,.cpp** - Handles adding, storing, deleting, updating... of weights between neurons.
* **IScapeInterface.h** - Interface for scape class, which provides inputs for training and scores outputs from neural networks.

##Usage
1. Implement training scape using IScapeInterface interface class, see **ExampleScape.h,.cpp**
2. Implement controller that controls flow of training process
* Constructing NeuralNetwork object is simple:
    ```C++
    NeuralNetwork *net = new NeuralNetwork(this);
    ```
* Next we need to set neural network's topology in vector format like follows:
    ```C++
    QVector<int> vec;
    vec.append(2);
    vec.append(1);
    vec.append(1);
    ```
    This network will have 2 input neurons, 1 hidden layer with 1 neuron and 1 output neuron.

* After that network is constructed with method construct:
    ```C++
    net->construct(vec);
    ```
* When network is scored after few attempts it emits signal and it should be recived in controller class:
    ```C++
    connect(net, &NeuralNetwork::trainingDone, this, &Controller::trainingDone, Qt::QueuedConnection);
    ```
* Before training we need to set-up training scape:
    ```C++
    Scape *scape = new Scape(net);
    net->setScape(scape);
    ```
3. After these steps API is ready to train neural networks.
