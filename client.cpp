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
    int option = 0;
    int patient = 0;
    double time = 0.000;
    int ecg = 0;
    char* req_filename = NULL;
    bool channel_request = false;

    while ((option = getopt(argc, argv, "p:t:e:f:c")) != -1) {
        switch (option) {
            case 'p':
                patient = atoi(optarg);
                break;
                
            case 't':
                time = atof(optarg);
                break;
                
            case 'e':
                ecg = atoi(optarg);    
                break; 
                        
            case 'f':
                req_filename = optarg;  
                break;
                
            case 'c':
                channel_request = true;
                break;
                
                  
        }
    }
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
        
        if (patient){        
            auto start = chrono::steady_clock::now();
            ofstream x_1("x1.csv");
            if (patient && !time && !ecg) {
                for (double i = 0.000; i <= 59.996; i+= 0.004){
                    datamsg ecg_1 = datamsg(patient, i, 1);
                    datamsg ecg_2 = datamsg(patient, i, 2);
                    chan.cwrite (&ecg_1, sizeof(ecg_1));
                    char* buf1 = chan.cread ();
                    chan.cwrite (&ecg_2, sizeof(ecg_2));
                    char* buf2 = chan.cread ();

                    if (x_1.is_open()){
                        x_1 << i << ',' << *((double*) buf1) << ',' <<  *((double*) buf2) << endl;
                    }
                }
            }
            else if (patient && time && ecg) {
                
                datamsg data = datamsg(patient, time, ecg);
                chan.cwrite (&data, sizeof(data));
                char* buf = chan.cread ();

                if (x_1.is_open()){
                    x_1 << time << "," << *((double*) buf) << endl;
                }
            }
            else if (patient && time && !ecg) {
                datamsg ecg_1 = datamsg(patient, time, ecg);
                chan.cwrite (&ecg_1, sizeof(ecg_1));
                char* buf1 = chan.cread ();
                datamsg ecg_2 = datamsg(patient, time, ecg);
                chan.cwrite (&ecg_2, sizeof(ecg_2));
                char* buf2 = chan.cread ();

                if (x_1.is_open()){
                    x_1 << time << ',' << *((double*) buf1) << ',' << *((double*) buf2);
                }
            }
            else if (patient && !time && ecg) {
                for (double i = 0.000; i <= 59.996; i+= 0.004){
                    datamsg data = datamsg(patient, i, ecg);
                    chan.cwrite (&data, sizeof(data));
                    char* buf = chan.cread ();

                    if (x_1.is_open()){
                        x_1 << i << ',' << *((double*) buf) << endl;
                    }
                }
            }
            x_1.close();
            auto end = chrono::steady_clock::now();
            int64_t elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            std::cout << elapsed << " milliseconds" << endl;
        }
        
        // End Task 1

        // Begin Task 2
        if (req_filename) {
            auto start = chrono::steady_clock::now();
            ofstream fout("y1.csv", ios::binary);

            string fname = req_filename;
            filemsg size_fm = filemsg(0, 0);
            int buf_size = sizeof(filemsg) + fname.length() + 1;
            char* size_msg = new char[buf_size];
            memcpy(size_msg, &size_fm, sizeof(filemsg));
            strcpy(size_msg + sizeof(filemsg), fname.c_str());
            chan.cwrite(size_msg, buf_size);
            __int64_t size = *(__int64_t*) chan.cread();


            for (int i = 0; i < size; i+= 256) {
                if (i + 256 < size) {
                    filemsg fm = filemsg(i, MAX_MESSAGE);
                    chan.cwrite(&fm, sizeof(fm));
                    fout.write(chan.cread(), MAX_MESSAGE);
                }
                else {
                    filemsg fm = filemsg(i, size % 256);
                    chan.cwrite(&fm, sizeof(fm));
                    fout.write(chan.cread(), size % 256);
                }

            }
            auto end = chrono::steady_clock::now();
            int64_t elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            std::cout << elapsed << " milliseconds" << endl;
        }
        // End Task 2

        // Begin Task 3

        if (channel_request){            
            MESSAGE_TYPE new_msg = NEWCHANNEL_MSG;
            chan.cwrite(&new_msg, sizeof(MESSAGE_TYPE));
            char* newChanName = chan.cread();
            FIFORequestChannel new_chan (newChanName, FIFORequestChannel::CLIENT_SIDE);
            datamsg test = datamsg(1, 0.008, 1);
            new_chan.cwrite(&test, sizeof(test));
            char* buf = new_chan.cread ();
            MESSAGE_TYPE m = QUIT_MSG;
            new_chan.cwrite(&m, sizeof(MESSAGE_TYPE));
        }
        // End Task 3

        // closing the channel    
        MESSAGE_TYPE m = QUIT_MSG;
        chan.cwrite (&m, sizeof (MESSAGE_TYPE));
        wait(&server_status);
    }   
}
