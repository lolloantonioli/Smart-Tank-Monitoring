#ifndef __CONTEXT__
#define __CONTEXT__

class Context {
  private:
    int manualLevel;
    bool buttonPressed;
    int remoteAutoLevel;
    bool remoteUnconnected;
    WCSState systemState;
    int valveLevel;
  public:
    Context();
    void setManualLevel(int level);
    int getManualLevel();
    void setButtonPressed(bool pressed);
    bool isButtonPressed();
    bool fetchButtonPressed(); 
    void setRemoteAutoLevel(int level);
    int getRemoteAutoLevel();
    void setRemoteUnconnected(bool status);
    bool isRemoteUnconnected();
    void setSystemState(WCSState state);
    WCSState getSystemState();
    void setValveLevel(int level);
    int getValveLevel();
}