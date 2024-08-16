from Engine.Engine import Engine, soc_Engine, file_Engine, win_process_Engine, win_socket_Engine, win_file_Engine, chrome_ext_Engine, processes_Engine
# from Engine.Engine import win_process_Engine
import threading
import time

if __name__=="__main__":
    EngineThread=threading.Thread(target=Engine,daemon=True)
    Soc_EngineThread=threading.Thread(target=soc_Engine,daemon=True)
    File_EngineThread=threading.Thread(target=file_Engine,daemon=True)
    win_process_EngineThread=threading.Thread(target=win_process_Engine,daemon=True)
    win_socket_EngineThread=threading.Thread(target=win_socket_Engine,daemon=True)
    win_file_EngineThread=threading.Thread(target=win_file_Engine,daemon=True)
    chrome_ext_EngineThread=threading.Thread(target=chrome_ext_Engine,daemon=True)
    processes_EngineThread=threading.Thread(target=processes_Engine,daemon=True)
    EngineThread.start()
    Soc_EngineThread.start()
    File_EngineThread.start()
    win_process_EngineThread.start()
    win_socket_EngineThread.start()
    win_file_EngineThread.start()
    chrome_ext_EngineThread.start()
    processes_EngineThread.start()
    time.sleep(1)
    while True:
        pass