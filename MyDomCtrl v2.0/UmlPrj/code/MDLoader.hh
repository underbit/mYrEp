#ifndef _MDLOADER_H
#define _MDLOADER_H


#include <string>
using namespace std;

//Il loader si occupa di caricare e salvare i dati relativi al sistema da e verso il file MD.conf
class MDLoader {
  private:
    //Il file che contiene le impostazioni del sistema
    string _fileSettings;


  public:
    bool CaricaImpostazioni();

    //Salva le impostazioni nel file di configurazione. Restituisce false in caso di problemi
    bool SalvaImpostazioni();


  private:
    MDSysCtrl ;

};
#endif
