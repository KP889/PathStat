// PathStat.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <thread>
#include<mutex>
#include"Pool.h"
#include <functional>


int main()
{
    auto hardware_threads_count = std::max(1u, std::thread::hardware_concurrency());


    std::vector<std::thread>threads;// (hardware_threads_count);
    Pool_auto P_obj;
   
    StatystykaPlikow stat("C:/Users/RetailAdmin/Desktop/kp");
    stat.skanujKatalog();
   

   
    int temp = stat.DlugoscStosu();
    //ustaw do kolejki iteratory do plikow

    for (int i = 0; i < hardware_threads_count; i++)
    {
        //auto func = std::bind(&A::func_1, &objA);
        threads.push_back(std::thread(std::bind(&Pool_auto::Infinite_loop_function, &P_obj)));
       
    }
        
    
    do
    {
       P_obj.Add_Job(std::bind(&StatystykaPlikow::Statystyka_plikow, &stat));
    } while (stat.DlugoscStosu() != 0);
        //P_obj.shutdown();
      
       std:: cout<<threads.at(0).joinable()<<"\n";
      
      // std::assert(!threads.at(0).joinable());
       P_obj.shutdown();
       for (auto o = 0; o < threads.size(); o++)      threads.at(o).join();
 
      
    
 
    //stat.Statystyka_plikow();
       
        stat.StatPlikow();
        stat.StatLinii();
        
    

    
    //std::cout << "Hello World!\n";
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
