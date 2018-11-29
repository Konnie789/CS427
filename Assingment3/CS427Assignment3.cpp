/* * * * * * * * * * * * * * * * *
* filename: CS427Assignment3.cpp *
* author: Konstantin Kharitonov  *
* SID: 200354502                 *
* class: CS 427                  *
* plz dont steal                 *
* * * * * * * * * * * * * * * * */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

/*Open a file by name, read its entire contents and loads them onto
arrays. Time data is loaded onto time_array, Amp loaded onto amp_array
Sample rate is saved in the sample_rate variable. */
unsigned readfile(string filename, vector<double> &time_array,vector<double> &amp_input){

  int length;
  char * buffer;
  string line, semicolon, word1, word2;
  unsigned sample_rate;

  double time, amplitude;


  if (!filename.empty()){
    ifstream file;
    file.open(filename, ios::binary);
    file.seekg(0, ios::end);
    length = file.tellg();
    file.seekg(0, ios::beg);

    getline(file, line);
    istringstream string_stream(line);
    string_stream >> semicolon >> word1 >> word2 >> sample_rate;
    getline(file, line);
    while(getline(file, line))
    {
      istringstream string_stream(line);
      string_stream >> time >> amplitude;
      time_array.push_back(time);
      amp_input.push_back(amplitude);
    }

    buffer = new char [length];
    file.read(buffer, length);
    file.close();

  }
  else{
      cout << "File not specified." << endl;
  }

  return sample_rate;
}
/* This function is a simple distortion algorithm that implements
the very basics of clipping. The limits of the clip is chosen
because 0.07 was one of the higher (and lower) samples that
sox created in the input dat file.
*/
void simple_distortion(vector<double> &amp_input, vector<double> &amp_output){

    double upper = 0.07;
    double lower = -0.07;

    for(int i = 0; i < amp_input.size(); i++){

      if(amp_input[i] > upper){
        amp_output.push_back(upper);
      }

      else if(amp_input[i] < lower){
        amp_output.push_back(lower);
      }
      else{
        amp_output.push_back(amp_input[i]);
      }
    }
}
/* This function is the implementation of a non-linear
distortion effect, similar to that of a guitar distortion
pedal. The formula used to determine the distortion effect is 
f(x) = blend * (x/|x|) * (1 - e^[gain*(x^2)/|x|])

The gain is adds to the amount of clipping that will occur at each
amplitude while the blend controls how distorted the effect gets. 
*/
void complex_distortion(vector<double> &amp_input, vector<double> &amp_output,
  double gain, double blend){

      double constant = 0;
      double exponent = 0;
      double func = 0;

      for(int i = 0; i < amp_input.size(); i++){

          constant = (amp_input[i] / abs(amp_input[i]));
          if(blend <= 0 || gain <= 0){
            func = amp_input[i];
          }
          else{
            exponent = gain * amp_input[i] * constant;
            func = blend * constant * (1 - exp(exponent));
          }
          amp_output.push_back(func);
        }
  }


/*Opens a file by name, and writes the sample_rate, time_array and
transformed amp_output onto said file. */
void writefile(string filename, vector<double> &time_array,vector<double> &amp_output,
unsigned sample_rate){

  int length;
  char * buffer;
  string line, semicolon, word1, word2;
  double time, amplitude;
  ofstream file;

  file.open(filename, ios::binary);
  file << "; Sample Rate " << sample_rate << endl;
  file << "; Channels 1" << endl;

  for(int i = 0; i < time_array.size(); i++){
    file << time_array[i] << " " << amp_output[i] << endl;
  }
}

//Where everything is put together
int main(int argc, char* argv[]){
  vector<double> time_array;
  vector<double> amp_input;
  vector<double> amp_output_simple;
  vector<double> amp_output_complex;
  double gain, blend;
  ofstream file;

  string str1 = argv[1];
  string str2 = argv[2];
  string str3 = argv[3];



  file.open(str2);
  cout << "How much gain do you want to add from 0 to 10? (0 = none, 10 = heavy)" << endl;
  cin >> gain;
  cout << "How much do you want to blend your signal? (0 = dry, 1 = wet)" << endl;
  cin >> blend;
  cout << "Algorithm Processing." << endl;
  unsigned sample_rate = readfile(str1, time_array, amp_input);


  simple_distortion(amp_input, amp_output_simple);
  complex_distortion(amp_input, amp_output_complex, gain, blend);

  writefile(str2, time_array, amp_output_simple, sample_rate);
    writefile(str3, time_array, amp_output_complex, sample_rate);
  unsigned samples = time_array.size();
  file.close();

  cout << endl << endl;
  cout << "Algorithm Completed." << endl;
  cout << "input file: " << argv[1] << endl;
  cout << "output file 1: " << argv[2] << endl;
  cout << "output file 2: " << argv[3] << endl;
  cout << "Sample rate = " << sample_rate << endl;
  cout << "Number of samples = " << samples << endl << endl;

  return 0;
}
