#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <fstream>
#include <chrono>

using std::cout;
using std::cin;
using std::string;
using std::bitset;
using std::endl;
using std::ifstream;
using std::ofstream;

string popas(string hex);
string xoras(string a,string b);
string andas(string a, string b);
string notas(string a);
string oras(string a, string b);

int main(int argc, char *argv[]){
    auto start = std::chrono::high_resolution_clock::now();
    string fraze;
    string ID[5]={"01000100111110010111000000110001","11010000010010000000000000000001","01000100111110010111011110110001","11011111110010000000011000111101","11010100111110010111000100110001"};
    string hex;
    bool debug = false;
    bool getout = true;
    bool debugas = false;
    string pavadinimas;
    ifstream inFile;
    ofstream ff;
    ff.open("log.txt", std::fstream::app);
    std::stringstream strStream;
    int a = 0;
    if(argc > 1){ //uzrasyta per command line
        for(int i = 1; i<argc;i++){
            fraze+=argv[i];
            fraze+=" ";
        }
    }
    else{
        char ats;
        while(getout){
        cout << "Skaityti is failo? (Y/N)";
        cin >> ats;
            switch(ats){
        case 'Y':
            getout = false;
            cout << "Iveskite failo pavadinima: ";
            cin >> pavadinimas;
            inFile.open(pavadinimas + ".txt");
            strStream << inFile.rdbuf();
            fraze = strStream.str();
            break;
        case 'y':
            getout = false;
            cout << "Iveskite failo pavadinima: ";
            cin >> pavadinimas;
            inFile.open(pavadinimas + ".txt");
            strStream << inFile.rdbuf();
            fraze = strStream.str();
            break;
        case 'N':
            getout = false;
            cout << "Iveskite norima teksta uzhashuoti: ";
            cin >> fraze;
            break;
        case 'n':
            getout = false;
            cout << "Iveskite norima teksta uzhashuoti: ";
            cin >> fraze;
            break;
        case '*':
            debugas = true;
            cout << "... DEBUGINIMO MODAS ... JEI NETYCIA IEJOTE ISJUNKITE IR IJUNKITE \n";
            debug = true;
            getout = false;
            cout << "Iveskite failo pavadinima: ";
            cin >> pavadinimas;
            start = std::chrono::high_resolution_clock::now();
            inFile.open(pavadinimas + ".txt");
            debug:
            getline(inFile,fraze);
            if(inFile.eof())
                debug = false;
            break;
        default:
            cout << "\n!!! Y y arba N n !!! \n";
            break;
        }
        }

            ///cout << "\nHashas: ";   ///

    }//rasyti ranka
        for(int i = 0; i<fraze.size();i++){
            hex += bitset<8>(fraze.c_str()[i]).to_string();
        }
        hex += "1";
        while(hex.size()%448!=0){
            hex+="0";
            a++;
        }
        for(int i = 0; i<56;i++, hex+="0");
        hex += bitset<32>(fraze.size()*8).to_string();
        int dydis = hex.size()/32;
        string hexiukai[dydis];
        string zodziai[80];
        for(int i = 0;i<dydis;i++){
            hexiukai[i]=popas(hex);
            hex.erase(hex.begin(),hex.begin()+32);
            //cout << hexiukai[i] << endl;
        }
        for(int i = 0, j = 2, p = 8, k = 13, h = 0; h < 80;h++,i++,j++,p++,k++){
            if(i > dydis-1) i=0;
            if(j > dydis-1) j=0;
            if(p > dydis-1) p=0;
            if(k > dydis-1) k=0;
            //cout << i << " " << j << " " << p << " " << k << endl;
          //  cout << h << endl;
            zodziai[h]=xoras(xoras(xoras(hexiukai[k],hexiukai[p]),hexiukai[j]),hexiukai[i]);
            zodziai[h]+=zodziai[h][0];
            zodziai[h].erase(zodziai[h].begin(),zodziai[h].begin()+1);
            //cout << zodziai[h] << endl;
        }
            for(int i = 0; i<5;i++){
                for(int j = 0; j<80;j++){
                    if(i%2==0){
                        ID[i]=andas(ID[i],notas(zodziai[j]));
                        ID[i]+=ID[i][0];
                        ID[i].erase(ID[i].begin(),ID[i].begin()+1);
                    }
                    else
                        ID[i]=oras(notas(ID[i]),zodziai[j]);

                }
            }
            for(int i = 0; i<5;i++){
            bitset<32> set(ID[i]);
            ff << std::hex << set.to_ulong();
            ////cout << std::hex << set.to_ulong();///
            }
    ff << "\n";
    ///cout << "\n";///
    if(debug == true)
        goto debug;
    if(debugas == true){
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    cout << "=== Hashavimas uztruko: " << diff.count() << "\n";
    }
return 0;
}

string popas(string hex){
    hex.erase(hex.begin()+32,hex.end());
    return hex;
}

string xoras(string a,string b){
    string ats;
    for(int i = 0; i < 32;i++){
        if(a[i]=='1' && b[i]=='0' || b[i]=='1' && a[i]=='0')
            ats+="1";
        else ats+="0";
    }
    return ats;
}
string andas(string a, string b){
    string ats;
    for(int i = 0; i < 32; i++){
        if(a[i]=='1' && b[i]=='1' || a[i]=='0' && b[i]=='0' )
            ats+="1";
        else ats+="0";
    }
    return ats;
}
string notas(string a){
    string ats;
    for(int i = 0; i < 32; i++){
        if(a[i]=='1')
            ats+="0";
        else ats+="1";
    }
    return ats;
}
string oras(string a, string b){
    string ats;
    for(int i = 0; i < 32; i++){
        if(a[i]=='1' || b[i]=='1')
            ats+="1";
        else ats+="0";
    }
    return ats;

}

