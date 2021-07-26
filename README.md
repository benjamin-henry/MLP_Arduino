# MLP_C
MLP Multi Layer Perceptron implementation ( for Arduino or other MCUs )

Tested on ESP32

Available Activations
```c
enum Activation {
    Sigmoid=0,
    Tanh,
    Relu,
    Softmax,
    Linear
};
```

Available Losses 
```c
enum Loss {
  Categorical_Crossentropy = 0,
  Binary_Crossentropy,
  MSE,
};
```

To see examples, please go to the examples folder.
