/* * * * * * * * * * * * * * * * *
* filename: CS427Assignment2.cpp *
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
#include <complex>

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

/*The Cooley-Tukey Algorithm splits both even and odd inputs are
computed seperately then combined after. As such, the amplitudes
are seperated into two different arrays.
*/
void even_odd(complex<double>* amp_array, unsigned samples){
    complex<double>* amp_temp = new complex<double>[samples/2];

     for(int i = 0; i < samples/2; i++){
        amp_temp[i] = amp_array[(i*2)+1];
     }

     for(int i = 0; i < samples/2; i++){
       amp_array[i] = amp_array[i*2];
     }

     for(int i = 0; i < samples/2; i++){
       amp_array[i + samples/2] = amp_temp[i];
     }

     delete[] amp_temp;
}

/* This is the Cooley-Tukey Algorithm in action. Seperation of the
two arrays needs to be done, so the seperation function is called here.
 Then the algorithm adds up the even and odd arrays such that the
 new obtained arrays of Xk (the new transformed amplitudes) can be
 ploted against frequencies.
*/
void algorithm(complex<double>* amp_array, unsigned samples, string str2){
     ofstream file;
     file.open(str2);

     if(samples < 2){

     }
     else{
     even_odd(amp_array, samples);
     algorithm(amp_array, samples/2, str2);
     algorithm(amp_array + samples/2,  samples/2, str2);

     for(int k = 0; k < samples/2; k++){
       complex<double> e_k = amp_array[k];
       complex<double> o_k = amp_array[k+samples/2];
       complex<double> w = exp(complex<double>(0,(-2. * M_PI * k/samples)));

       amp_array[k] = e_k + w * o_k;
       amp_array[k + samples/2] = e_k - w * o_k;
     }

     for(int k = 0; k < samples; k++){
       file << fixed << k << "," << abs(amp_array[k]) << endl;
     }
   }
}

//Where everything is put together
int main(int argc, char* argv[]){
  vector<double> time_array;
  vector<double> amp_input;
  ofstream file;
  string str1 = argv[1];
  string str2 = argv[2];

  file.open(str2);
  unsigned sample_rate = readfile(str1, time_array, amp_input);
  unsigned samples = time_array.size();
  complex<double> amp_array[samples];

  for(int i = 0; i < samples; i++){
      amp_array[i] = amp_input[i];
  }

  cout << "Algorithm running " << endl << endl;
  algorithm(amp_array, samples, str2);
  file.close();

  cout << "Algorithm Completed." << endl;
  cout << "input file: " << argv[1] << endl;
  cout << "output file: " << argv[2] << endl;
  cout << "Sample rate = " << sample_rate << endl;
  cout << "Number of samples = " << samples << endl << endl;


  return 0;
}
