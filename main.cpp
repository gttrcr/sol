#include <iostream>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <climits>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <random>

#define SMALLEST_CPP_TYPE unsigned char
#define BIGGEST_CPP_TYPE unsigned long long int
#define OUTPUT_PRECISION 20

#ifdef _WIN32
#include <intrin.h>
#include <windows.h>

uint64_t rdtsc()
{
    return __rdtsc();
}

BIGGEST_CPP_TYPE get_available_system_memory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
}
#else
#include <x86intrin.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

uint64_t rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

BIGGEST_CPP_TYPE get_available_system_memory()
{
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    return memInfo.freeram;
}
#endif

const unsigned int thread_n = std::thread::hardware_concurrency();
const BIGGEST_CPP_TYPE max_universe_size = get_available_system_memory() / sizeof(SMALLEST_CPP_TYPE);

void cpucycle_monitor(unsigned int *counter)
{
    unsigned int last = 0;
    while (true)
    {
        unsigned int current = rdtsc();
        if (last > current)
            counter[1]++;
        counter[0] = current;
        last = current;
    }
}

SMALLEST_CPP_TYPE *universe;

template <typename T>
void light_travel(unsigned int *counter)
{
    T pos = 0;
    unsigned int start[2];
    unsigned int stop[2];
    std::chrono::_V2::system_clock::time_point start_time;
    memset(start, 0, sizeof(start));
    memset(stop, 0, sizeof(stop));
    std::ofstream output("c");

    while (universe == nullptr)
        std::cout << "Waiting for the end of creation..." << std::endl;

    while (true)
    {
        universe[pos - 1] = 0;
        universe[pos] = 1;
        if (pos == 0 && stop[0] == 0 && stop[1] == 0)
        {
            if (start[0] != 0 && start[0] != 0)
            {
                stop[0] = counter[0];
                stop[1] = counter[1];

                double s_delta = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000000000.0;
                double c = (pow(2.0, 32.0) * ((double)stop[1] - (double)start[1]) + ((double)stop[0] - (double)start[0])) / s_delta;
                // std::cout << c << std::endl;
                output << std::setprecision(OUTPUT_PRECISION) << c << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            memset(start, 0, sizeof(start));
            memset(stop, 0, sizeof(stop));
            start[0] = counter[0];
            start[1] = counter[1];
            start_time = std::chrono::high_resolution_clock::now();
        }

        pos++;
    }

    output.close();
}

std::vector<std::string> split_string(const std::string &str, const char delim)
{
    std::vector<std::string> ret;
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim))
        ret.push_back(s);

    return ret;
}

std::string help()
{
    return "help: print this help\n"
           "saturate t: saturate the computational power of the universe for t time";
}

void while_true(const BIGGEST_CPP_TYPE &t)
{
    std::chrono::_V2::system_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_time).count() < t)
    {
    }
}

void saturate(const std::vector<std::string> &args)
{
    BIGGEST_CPP_TYPE t = 5000000000;
    for (unsigned int i = 0; i < thread_n; i++)
        std::thread(while_true, t).detach();
}

void move()
{
    unsigned int *universe = new unsigned int[100];
    memset(universe, 0, 100 * sizeof(*universe));

    const unsigned int start = 0;
    const unsigned int mass = 5;
    for (unsigned int i = start; i < start + mass; i++)
        universe[i] = 1;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, mass - 1); // distribution in range [1, 6]

    while (true)
    {
        unsigned int rand = dist(rng);
        if (universe[rand] == 1)
            universe[rand] = 0;
        else if (universe[rand] == 0 && universe[rand + 1] == 0)
            universe[rand + 1] = 1;

        for (unsigned int i = 0; i < 100; i++)
            std::cout << universe[i];
        std::cout << std::endl;

        bool m = true;
        for (unsigned int i = start + 1; i < start + 1 + mass && m; i++)
            m &= universe[i] == 1;

        if (m)
            break;
    }
}

int main()
{
    move();
    std::cout << std::setprecision(OUTPUT_PRECISION) << "Parallelization: " << thread_n << std::endl;
    std::cout << "\tTime (1)" << std::endl;
    std::cout << "\tSingle photo simulation (1)" << std::endl;
    std::cout << "\tGod (1)" << std::endl;
    std::cout << "Max Universe size: " << max_universe_size << std::endl;
    std::cout << "Let there be time..." << std::endl;
    unsigned int *counter = new unsigned int[2];
    memset(counter, 0, 2 * sizeof(*counter));
    std::thread cpucycle_monitor_thread(cpucycle_monitor, counter);
    cpucycle_monitor_thread.detach();
    std::cout << "Time created" << std::endl;

    std::thread light_travel_simulation;
    unsigned long long observable_universe_size;
    if (max_universe_size > ULLONG_MAX)
    {
        observable_universe_size = ULLONG_MAX;
        light_travel_simulation = std::thread(light_travel<unsigned long long int>, counter);
    }
    else if (max_universe_size > ULONG_MAX)
    {
        observable_universe_size = ULONG_MAX;
        light_travel_simulation = std::thread(light_travel<unsigned long int>, counter);
    }
    else if (max_universe_size > UINT_MAX)
    {
        observable_universe_size = UINT_MAX;
        light_travel_simulation = std::thread(light_travel<unsigned int>, counter);
    }
    else if (max_universe_size > USHRT_MAX)
    {
        observable_universe_size = USHRT_MAX;
        light_travel_simulation = std::thread(light_travel<unsigned short int>, counter);
    }
    else if (max_universe_size > UCHAR_MAX)
    {
        observable_universe_size = UCHAR_MAX;
        light_travel_simulation = std::thread(light_travel<unsigned char>, counter);
    }
    else
        return -1;

    light_travel_simulation.detach();

    std::cout << "Creating space..." << std::endl;
    universe = new SMALLEST_CPP_TYPE[observable_universe_size];
    memset(universe, 0, observable_universe_size * sizeof(*universe));
    std::cout << "Space created" << std::endl;

    std::cout << "Observable universe size: " << observable_universe_size << std::endl;

    while (true)
    {
        std::cout << "con-sol-e$ ";
        std::string command;
        std::cin >> command;
        if (command == "help")
            std::cout << help() << std::endl;
        else if (command == "saturate")
            saturate(split_string(command, ' '));
        else
            std::cout << "Unknown command " << command << std::endl
                      << "Please consider the following list of commands" << std::endl
                      << help() << std::endl;
    }
}