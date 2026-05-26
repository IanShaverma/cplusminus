Тестовое задание состоит из 3х частей:
1) scan_proc.cpp - сканирует информацию о процессоре из системного файла /proc/stat. В моем случае используется заглушка.
2) udp_client.cpp - отправляет std::vector<double> по UDP
3) udp_server.cpp - принимает std::vector<double> по UDP

Для корректного запуска необходимо выполнить 2 команды:
g++ ./src/scan_proc.cpp udp_client.cpp -o main
g++ ./src/udp_server.cpp -o server
а затем запустить файлы ./main и ./server в консоли.

Пример работы программы:
./main:
  Vector sent.
  All CPU usage: 0%
  CPU -0- 4%
  CPU -1- 0%
  CPU -2- 0%
  CPU -3- 100%
  CPU -4- 100%
  CPU -5- 6%
  CPU -6- 72%
  CPU -7- 26%

./server:
  Received: 0 4 0 0 100 100 6 72 26 

  Сборка проекта при помощи cmake:
    cmake -S . -B build
    cmake --build build
