/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/19
 */
#include "common.h"
#include "FIFOreqchannel.h"
#include <fstream>
#include <chrono>

using namespace std;


int main(int argc, char *argv[]){
    int n = 100;    // default number of requests per "patient"
	int p = 15;		// number of patients
    srand(time_t(NULL));

    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

    // Begin Task 1
    auto start = chrono::steady_clock::now();
    ofstream x_1("x1.csv");
    for (double i = 0.000; i <= 59.996; i+= 0.004){
        datamsg ecg_1 = datamsg(1, i, 1);
        datamsg ecg_2 = datamsg(1, i, 2);
        chan.cwrite (&ecg_1, sizeof(ecg_1));
        char* buf1 = chan.cread ();
        chan.cwrite (&ecg_2, sizeof(ecg_2));
        char* buf2 = chan.cread ();

        if (x_1.is_open()){
            x_1 << i << "," << *((double*) buf1) << "," << *((double*) buf2) << endl;
        }
    }
    x_1.close();
    auto end = chrono::steady_clock::now();
    int64_t elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << elapsed << " milliseconds" << endl;
    // End Task 1

    // Begin Task 2

    // closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite (&m, sizeof (MESSAGE_TYPE));

   
}
