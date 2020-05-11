#include <iostream>
#include <vector>
#include <fstream>
#include "plant.h"

using namespace std;

void input_from_file(size_t& N, size_t& k, vector<size_t>& M, vector<double>& a, vector<double>& b, vector<double>& g) {
    ifstream input_file("input");

    input_file >> N >> k;
    M.resize(k);
    for (size_t i = 0; i < k; i++)
    {
        input_file >> M[i];
    }

    a.resize(k);
    b.resize(k);
    g.resize(k);

    for (size_t i = 0; i < k; i++)
    {
        input_file >> a[i] >> b[i] >> g[i];
    }
}

void input_from_keyboard(size_t& N, size_t& k, vector<size_t>& M, vector<double>& a, vector<double>& b, vector<double>& g) {
    cout << "Enter number of measures:";
    cin >> N;

    cout << "Enter number of channels:";
    cin >> k;

    M.resize(k);
    cout << "Enter channels:";
    for (size_t i = 0; i < k; i++)
    {
        cin >> M[i];
    }

    a.resize(k);
    b.resize(k);
    g.resize(k);
    cout << "Enter constants a, b, g:";
    for (size_t i = 0; i < k; i++)
    {
        cin >> a[i] >> b[i] >> g[i];
    }
}

double sr_znach(const vector<double>& Xi, const size_t& N) {
    double sum = 0;
    for (const auto& elem : Xi)
    {
        sum += elem;
    }
    return sum/N;
}

vector<double> take_one_chanel(const vector<vector<double>>& all_chanels, const int& chanel_number) {
    vector<double> result;
    for (const auto item : all_chanels)
    {
        result.push_back(item[chanel_number]);
    }
    return result;
}

double TEP1(const double& ai, const vector<double>& Xi, const size_t& N) {
    return ai * sr_znach(Xi, N);
}

double TEP2(const double& bi, const double& gi, const vector<double>& Xi, const size_t& N) {
    vector<double> new_Xi;
    for (const auto& elem : Xi)
    {
        new_Xi.push_back((elem - gi) * (elem - gi));
    }
    return bi*sr_znach(new_Xi, N);
}

void output_headline(const size_t& k) {
    cout << "num   |";
    for (size_t i = 0; i < k; i++)
    {
        cout << "    X[" << i << "]    |";
    }
    cout << endl;
}

void output(const vector<vector<double>>& X, const vector<vector<double>>& TEP1_result, const vector<vector<double>>& TEP2_result, const size_t& N, const size_t& k) {

    output_headline(k);

    for (size_t i = 0; i < N; i++)
    {
        cout << i + 1 << "     |";

        for (size_t j = 0; j < k; j++)
        {
            cout << fixed;
            cout.precision(3);
            cout << "   " << X[i][j] << "   |";
        }
        cout << endl;
    }
    cout << endl;
    cout << "TEP1 OUTPUT" << endl;

    output_headline(k);

    for (size_t i = 0; i < N; i++)
    {
        cout << i + 1 << "     |";

        for (size_t j = 0; j < k; j++)
        {
            cout << fixed;
            cout.precision(3);
            cout << "   " << TEP1_result[j][i] << "   |";
        }
        cout << endl;
    }

    cout << endl;
    cout << "TEP2 OUTPUT" << endl;

    output_headline(k);

    for (size_t i = 0; i < N; i++)
    {
        cout << i + 1 << "     |";

        for (size_t j = 0; j < k; j++)
        {
            cout << fixed;
            cout.precision(3);
            cout << "  " << TEP2_result[j][i] << "  |";
        }
        cout << endl;
    }

}

int main() {
    Plant plant;
    plant_init(plant);

    size_t N;
    size_t k;
    vector<size_t> M;
    vector<double> a, b, g;

    //input_from_file(N, k, M, a, b, g);

    input_from_keyboard(N, k, M, a, b, g);

    vector<vector<double>> X;

    vector<vector<double>> TEP1_result(k);
    vector<vector<double>> TEP2_result(k);

    for (size_t i = 0; i < N; i++)
    {
        X.resize(i+1);
        for (size_t j = 0; j < k; j++)
        {
            X[i].push_back(plant_measure(M[j], plant));
            TEP1_result[j].push_back(TEP1(a[j], take_one_chanel(X, j), i+1));
            TEP2_result[j].push_back(TEP2(b[j], g[j], take_one_chanel(X, j), i+1));
        }
    }
    output(X, TEP1_result, TEP2_result, N, k);
    return 0;
}
