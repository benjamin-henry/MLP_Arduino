#include <MLP.h>

unsigned int input_shape = 3;
unsigned int layers_cfg[][2] = {
  {12, Relu},
  {3, Linear},
};

// some accelerometer data
float X[][3] = {
  {0.003f, -0.010f,0.975f}, // Z+
  {-0.006f, .985f,-0.025f}, // Y+
  {.99f, -.015f,-.025f}, // X+
  {-.019f, -.034f,-1.025f}, // Z-
  {0.004f, -1.015f,-.030f}, // Y-
  {-1.014f, .003f,-.030f} // X-
};

float y_true[][3] = {
  {0.f, 0.f, 1.f},
  {0.f, 1.f, 0.f},
  {1.f, 0.f, 0.f},
  {0.f, 0.f, -1.f},
  {0.f, -1.f, 0.f},
  {-1.f, 0.f, 0.f},
  
};

float input_buff[3] = {0.f};
float output_buff[3] = {0.f};

float learning_rate = .015f;

MLP mlp = mlp_from_cfg(input_shape, sizeof(layers_cfg) / sizeof(layers_cfg[0]), layers_cfg);


void setup() {
  Serial.begin(115200);
  randomize_mlp(mlp);

  unsigned int batch_size = sizeof(X) / sizeof(X[0]);
  unsigned int output_shape = mlp->_layers[mlp->_n_layers - 1]->_units;
  unsigned int cnt = 99;

  Serial.print("batch");
  Serial.print('\t');
  Serial.println("loss");
  
  for (unsigned int i = 0; i < 10000; i++) {
    float loss = train_on_batch(mlp, batch_size, output_shape, (float*)X, (float*)y_true, MSE, learning_rate);
    if (!cnt) {
      cnt = 99;
      Serial.print(i + 1);
      Serial.print('\t');
      Serial.println(loss, 12);
    } else {
      cnt--;
    }
  }

  Serial.println();

  if(!EEPROM.begin(1024)) {ESP.restart();}

  mlp_save_weights(mlp, 0);  
  mlp_load_weights(mlp, 0);

  EEPROM.end();


  Serial.print("y_true");
  Serial.print('\t');
  Serial.println("y_pred");
  for (unsigned int i_sample = 0; i_sample < batch_size; i_sample++) {
    extract1dfrom2d((float*)X, (float*)input_buff, i_sample , mlp->_input_shape);
    mlp_predict(mlp, input_buff, output_buff);
    for (int j = 0; j < output_shape; j++) {
      Serial.print(y_true[i_sample][j], 3);
      Serial.print('\t');
      Serial.println(output_buff[j], 3);
    }
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}