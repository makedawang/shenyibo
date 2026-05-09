#include "PCFG.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include "md5.h"
#include <iomanip>
using namespace std;
using namespace chrono;

int main()
{
    cout << "Testing MD5Hash correctness..." << endl;
    string test_pws[8] = {"123456","password","12345678","qwerty","123456789","12345","1234","111111"};
    string test_hashes[8] = {"e10adc3949ba59abbe56e057f20f883e","5f4dcc3b5aa765d61d8327deb882cf99","25d55ad283aa400af464c76d713c07ad","d8578edf8458ce06fbc5bb76a58c5ca4","25f9e794323b453885f5181f1b624d0b","827ccb0eea8a706c4c34a16891f84e7b","81dc9bdb52d04dc20036dbd8313ed055","96e79218965eb72c92a549dd5a330112"};
    
    bool pass = true;
    for(int i=0;i<8;i++){
        bit32 state[4];
        MD5Hash(test_pws[i],state);
        stringstream ss;
        for(int i1=0;i1<4;i1++)ss<<hex<<setw(8)<<setfill('0')<<state[i1];
        if(ss.str()!=test_hashes[i]){
            cerr<<"MD5 test failed: "<<test_pws[i]<<endl;
            pass = false;
        }
    }
    if(pass) cout << "MD5Hash test passed!" << endl;

    // 修复：原工程类名是 PriorityQueue，不是 PCFG！
    double time_hash=0,time_guess=0,time_train=0;
    PriorityQueue q;
    auto start_train=system_clock::now();
    q.m.train("/guessdata/Rockyou-singleLined-full.txt");
    q.m.order();
    auto end_train=system_clock::now();
    time_train=double(duration_cast<microseconds>(end_train-start_train).count())*microseconds::period::num/microseconds::period::den;
    q.init();

    int curr_num=0,history=0;
    auto start=system_clock::now();
    while(!q.priority.empty()){
        q.PopNext();
        q.total_guesses=q.guesses.size();
        if(q.total_guesses-curr_num>=100000){
            cout<<"Guesses generated: "<<history+q.total_guesses<<endl;
            curr_num=q.total_guesses;
            if(history+q.total_guesses>10000000){
                auto end=system_clock::now();
                time_guess=double(duration_cast<microseconds>(end-start).count())*microseconds::period::num/microseconds::period::den;
                cout<<"Guess time:"<<time_guess-time_hash<<"seconds"<<endl;
                cout<<"Hash time:"<<time_hash<<"seconds"<<endl;
                cout<<"Train time:"<<time_train<<"seconds"<<endl;
                break;
            }
        }
        if(curr_num>1000000){
            auto start_hash=system_clock::now();
            int n=q.guesses.size();
            for(int i=0;i<n;i+=4){
                const char* inputs[4]={nullptr};
                int cnt=0;
                for(int j=0;j<4&&i+j<n;j++){inputs[j]=q.guesses[i+j].c_str();cnt++;}
                if(cnt==4){
                    unsigned char outputs[4][16];
                    md5_neon(inputs,outputs);
                }else{
                    bit32 state[4];
                    for(int j=0;j<cnt;j++)MD5Hash(inputs[j],state);
                }
            }
            auto end_hash=system_clock::now();
            time_hash+=double(duration_cast<microseconds>(end_hash-start_hash).count())*microseconds::period::num/microseconds::period::den;
            history+=curr_num;curr_num=0;q.guesses.clear();
        }
    }
    return 0;
}
