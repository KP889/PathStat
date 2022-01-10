#pragma once
#include<queue>
#include<mutex>
#include<vector>
#include<stack>
#include<filesystem>
#include<algorithm>
#include<utility>
#include <iostream>
#include<string>
#include<map>
#include <fstream>
#include<functional>
#include <atomic>


class Pool_auto
{private:
    //kolejka z pathami do plikow z klasy statystyka
    std::mutex queue_mutex;
    std::queue <std::function<void()>> q_listaPlikow;
    std::atomic<bool> terminate_pool;
    std::condition_variable condition;
    
public:
   // typedef void (ITask::* task_func)(void*, int);
    ~Pool_auto() {};
    void Infinite_loop_function()
    {
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);

                condition.wait(lock, [this]() {
                    return !q_listaPlikow.empty()|| terminate_pool; // dopisz wyjscie z watku
                    }); //uspij watek
                if (!q_listaPlikow.empty())
                {
                    
                    std::function Job = q_listaPlikow.front();// jakiego typu jest job?
                    q_listaPlikow.pop();
                    //std::cout << "ID watku: " << std::this_thread::get_id() << std::endl;
                    Job();
             
                }
                else
                {
                    return;
                }
            }
           
           
        }
    };
    void shutdown()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);//threadpool_mutex);
            terminate_pool = true; // use this flag in condition.wait
        }

        condition.notify_all(); // wake up all threads.

        // Join all threads.
        //for (std::thread& th : threads)
        //{
          //  th.join();
        //}

        //pool.clear();
        //stopped = true; // use this flag in destructor, if not set, call shutdown() 
    }
    void Add_Job(std::function<void()> New_Job)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            q_listaPlikow.push(New_Job);
        }

        condition.notify_one(); //wybudz watek
       
    }
    void F_testowa()
    {
        std::function Job = q_listaPlikow.front();// jakiego typu jest job?
        q_listaPlikow.pop();
        Job();
    }
    void DlugosListy() {
        std::cout << "Dlugosc listy: " << q_listaPlikow.size() << std::endl;
    }
};

class StatystykaPlikow
{ private:
    std::stack<std::filesystem::path> p_stos; //stos z path do znalezionych plikow
    std::vector <std::string> s_nFolderow;// vector z nazwami podfolderow
    std::vector<std::filesystem::path> nFolderow; // 
    std::map <std::string, int> statystyka_Plik{}; // kontener nazwa pliku i statystyka linii
    std::map<std::string, std::map<int, int>> statystyka_Rozbudowana; //nazwa linie puste linie pelne
    std::filesystem::path sciezkaP;
    std::filesystem::recursive_directory_iterator i_directory;
    int i_lPlikow = 0;
public:
    //konstruktor
    explicit StatystykaPlikow(std::filesystem::path sciezkaDostepu) : sciezkaP{ sciezkaDostepu }, i_directory(sciezkaP){};

    void Statystyka_plikow() {
        std::fstream plik;
        std::string nazwa;
        int liczba_linii;
        //do {
        if (!p_stos.empty()) 
        {
            std::filesystem::path my_path{ p_stos.top() };
            plik.open(p_stos.top());
            std::istreambuf_iterator<char> str_plik{ plik };
            std::istreambuf_iterator<char> str_plik_end;
            liczba_linii = count(str_plik, str_plik_end, '\n');
            nazwa = my_path.u8string();
            statystyka_Plik.insert(make_pair(nazwa, liczba_linii));
            plik.close();
            p_stos.pop();

         }
            //nazwa =static_cast<string> (my_path.filename();

        //} while (!(p_stos.empty()));
        


    }
    int DlugoscStosu() { return p_stos.size(); }
    
    void StatLinii() {

        for (auto [nazwa, l_linii] :  statystyka_Plik)std::cout << "Plik " << nazwa << " zawiera lini " << l_linii << "\n";

    }
    void StatPlikow() {
        std::cout << "Folder " << sciezkaP.filename() << " zawiera " << i_lPlikow << " pliki.\n ";
    }
    void skanujKatalog() {
        //std::vector<std::filesystem::path> nFolderow;
        //std::stack<std::filesystem::path> it_Plik;
        //pair< vector<filesystem::path>, stack<filesystem::path>> kontener;
        for (auto it : i_directory)
        {
            if (it.is_directory())
            {
                std::filesystem::path p_plik{ it };
                nFolderow.push_back(p_plik.filename());

            }
            else if (it.is_regular_file())
            {
                std::filesystem::path i_plik{ it };
                p_stos.push(i_plik);
                i_lPlikow++;
            }


        };
        //std::cout << "Statystyka folderu " << sciezkaP.filename() << " podfolderow: " << nFolderow.size() << " plikow " << p_stos.size() <<std:: endl;
        //return kontener_wyniki{ kontener.first }, stosPliki{kontener.second};
       



    };
};
