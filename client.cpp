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
#include <string.h>

using namespace std;


int main(int argc, char *argv[]){
    int server_status;
    if(!fork()){
        execvp("./dataserver", argv);
    }
    else {
        int n = 100;    // default number of requests per "patient"
        int p = 15;		// number of patients
        srand(time_t(NULL));

        FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

        // Begin Task 1 (on windows use --strip-trailing-cr flag with diff)
        /*
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
                x_1 << i << "," << *((double*) buf1) << "," << *((double*) buf2) << "\n";
            }
        }
        x_1.close();
        auto end = chrono::steady_clock::now();
        int64_t elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << elapsed << " milliseconds" << endl;
        */
        // End Task 1

        // Begin Task 2
        // auto start = chrono::steady_clock::now();
        // ofstream fout("bintest", ios::binary);

        // string fname = "test1";
        // filemsg size_fm = filemsg(0, 0);
        // int buf_size = sizeof(filemsg) + fname.length() + 1;
        // char* size_msg = new char[buf_size];
        // memcpy(size_msg, &size_fm, sizeof(filemsg));
        // strcpy(size_msg + sizeof(filemsg), fname.c_str());
        // chan.cwrite(size_msg, buf_size);
        // __int64_t size = *(__int64_t*) chan.cread();


        // for (int i = 0; i < size; i+= 256) {
        //     if (i + 256 < size) {
        //         filemsg fm = filemsg(i, MAX_MESSAGE);
        //         chan.cwrite(&fm, sizeof(fm));
        //         fout.write(chan.cread(), MAX_MESSAGE);
        //     }
        //     else {
        //         filemsg fm = filemsg(i, size % 256);
        //         chan.cwrite(&fm, sizeof(fm));
        //         fout.write(chan.cread(), size % 256);
        //     }

        // }
        // auto end = chrono::steady_clock::now();
        // int64_t elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        // cout << elapsed << " milliseconds" << endl;
        // End Task 2

        // Begin Task 3
        // MESSAGE_TYPE new_msg = NEWCHANNEL_MSG;
        // chan.cwrite(&new_msg, sizeof(MESSAGE_TYPE));
        // char* newChanName = chan.cread();
        // FIFORequestChannel new_chan (newChanName, FIFORequestChannel::CLIENT_SIDE);
        // datamsg test = datamsg(1, 0.008, 1);
        // new_chan.cwrite(&test, sizeof(test));
        // char* buf = new_chan.cread ();
        // cout << *((double*)buf) << "\n";
        // End Task 3

        // closing the channel    
        MESSAGE_TYPE m = QUIT_MSG;
        chan.cwrite (&m, sizeof (MESSAGE_TYPE));
        // new_chan.cwrite(&m, sizeof(MESSAGE_TYPE));
        wait(&server_status);
    }   
}
