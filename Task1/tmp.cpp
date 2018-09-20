    #include <iostream>
    #include <chrono>
    int main() {
      auto begin = std::chrono::steady_clock::now();
      getchar(); 
      auto end = std::chrono::steady_clock::now();
      
      auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    }