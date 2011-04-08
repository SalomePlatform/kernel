#include "Launcher.hxx"

int main(int argc, char** argv)
{

  try {
    Launcher_cpp *lcpp = new Launcher_cpp();
    ResourcesManager_cpp *rcpp = new ResourcesManager_cpp();
    lcpp->SetResourcesManager(rcpp);
    delete lcpp;
    delete rcpp;
    std::cout << "test OK" << std::endl;
  } catch ( const ResourcesException &ex) {
    std::cout << ex.msg.c_str() << std::endl;
  }
  catch ( const LauncherException &ex) {
    std::cout << ex.msg.c_str() << std::endl;
  }
  catch (...){
    std::cout << "test KO" << std::endl;
  }
  exit(0);
}
