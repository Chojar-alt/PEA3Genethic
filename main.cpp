#include <bits/stdc++.h>
#include <climits>
#include <fstream>
#include <sys/time.h>
using namespace std;

int n;
string filename;

#define V INT_MAX
#define POP_SIZE n

struct individual {
    string gnome;
    int fitness;
};


int rand_num(int start, int end)
{
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}


bool repeat(string s, char ch)
{
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ch)
            return true;
    }
    return false;
}

string mutatedGene(string gnome)
{
    while (true) {
        int r = rand_num(1, V);
        int r1 = rand_num(1, V);
        if (r1 != r) {
            char temp = gnome[r];
            gnome[r] = gnome[r1];
            gnome[r1] = temp;
            break;
        }
    }
    return gnome;
}

string create_gnome()
{
    string gnome = "0";
    while (true) {
        if (gnome.size() == V) {
            gnome += gnome[0];
            break;
        }
        int temp = rand_num(1, V);
        if (!repeat(gnome, (char)(temp + 48)))
            gnome += (char)(temp + 48);
    }
    return gnome;
}


int cal_fitness(string gnome)
{
    fstream();

    ifstream plik("br17.atsp");

    plik >> n;

    int** tab;
    tab = new int* [n];
    for (int i = 0; i < n; i++)
    {
        tab[i] = new int[n];
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            plik >> tab[i][j];
        }
    }
    int f = 0;
    for (int i = 0; i < gnome.size() - 1; i++) {
        if (tab[gnome[i] - 48][gnome[i + 1] - 48] == INT_MAX)
            return INT_MAX;
        f += tab[gnome[i] - 48][gnome[i + 1] - 48];
    }
    return f;
}


int cooldown(int temp)
{
    return (90 * temp) / 100;
}


bool lessthan(struct individual t1,
              struct individual t2)
{
    return t1.fitness < t2.fitness;
}


void TSPUtil(int** tab)
{
    // Generation Number
    int gen = 1;
    // Number of Gene Iterations
    int gen_thres = INT_MAX;

    vector<struct individual> population;
    struct individual temp;

    // Populating the GNOME pool.
    for (int i = 0; i < POP_SIZE; i++) {
        temp.gnome = create_gnome();
        temp.fitness = cal_fitness(temp.gnome);
        population.push_back(temp);
    }

    cout << "\nInitial population: " << endl
         << "GNOME     FITNESS VALUE\n";
    for (int i = 0; i < POP_SIZE; i++)
        cout << population[i].gnome << " "
             << population[i].fitness << endl;
    cout << "\n";

    int temperature = 10000;


    while (temperature > 1000 && gen <= gen_thres) {
        sort(population.begin(), population.end(), lessthan);
        cout << "\nCurrent temp: " << temperature << "\n";
        vector<struct individual> new_population;

        for (int i = 0; i < POP_SIZE; i++) {
            struct individual p1 = population[i];

            while (true) {
                string new_g = mutatedGene(p1.gnome);
                struct individual new_gnome;
                new_gnome.gnome = new_g;
                new_gnome.fitness = cal_fitness(new_gnome.gnome);

                if (new_gnome.fitness <= population[i].fitness) {
                    new_population.push_back(new_gnome);
                    break;
                }
                else {


                    float prob = pow(2.7,
                                     -1 * ((float)(new_gnome.fitness
                                                   - population[i].fitness)
                                           / temperature));
                    if (prob > 0.5) {
                        new_population.push_back(new_gnome);
                        break;
                    }
                }
            }
        }

        temperature = cooldown(temperature);
        population = new_population;
        cout << "Generation " << gen << " \n";
        cout << "GNOME     FITNESS VALUE\n";

        for (int i = 0; i < POP_SIZE; i++)
            cout << population[i].gnome << " "
                 << population[i].fitness << endl;
        gen++;
    }
}

int main()
{
   // cout<<"Wprowadz nazwe pliku: \n";
   // cin>>filename;
    //filename = "testFile/"+filename;
    filename = "testFile/br17.atsp";
    fstream();
    ifstream file(filename);
    file >> n;

    int** tab;
    tab = new int* [n];
    for (int i = 0; i < n; i++)
    {
        tab[i] = new int[n];
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            file >> tab[i][j];
        }
    }
    struct timeval start, end;
    long mtime, secs, usecs;

    gettimeofday(&start, NULL);

    TSPUtil(tab);
    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 10 + usecs/10.0) + 0.5;
    printf("Time: %ld ms\n", mtime);
}