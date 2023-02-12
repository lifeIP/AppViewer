import win32ui
import win32gui
from PIL import Image
import socket


class SvICON():
        
    def save_icon(self, Path, Name):
        large, small = win32gui.ExtractIconEx(Path, 0)
        win32gui.DestroyIcon(small[0])
        self.bitmapFromHIcon(large[0], Name)
        
    def bitmapFromHIcon(self, hIcon, sName):
         hdc = win32ui.CreateDCFromHandle(win32gui.GetDC(0))
         hbmp = win32ui.CreateBitmap()
         hbmp.CreateCompatibleBitmap(hdc, 32, 32)
         hdc = hdc.CreateCompatibleDC()
         hdc.SelectObject(hbmp)
         hdc.DrawIcon((0, 0), hIcon)
         hdc.DeleteDC()
         bmpstr=hbmp.GetBitmapBits(True)
         img = Image.frombuffer( 'RGBA',(32,32), bmpstr, 'raw', 'BGRA', 0, 1)
         img.save(sName)


    def readANDfind_for_many(self):
        with open('icons_info.iof', 'r', encoding="utf-8") as file:
        
            while 1:
                
                if(len(file.readline()) < 3):
                    break
                
                Name = file.readline()
                Path = file.readline()
            
                Path = Path.rstrip('\n')
                Name = Name.rstrip('\n')
                self.save_icon(Path, Name)
                
    def readANDfind_for_once(self, Path, Name):
        self.save_icon(Path, Name)
        




def start_work(ip, sock):
    print("Работа запущена0")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
        client.connect((ip, sock))        
        sv = SvICON()
        print("Работа запущена")
        while 1:
            i = 0
            data = client.recv(1024).decode('utf-8')
            while(len(data) > i):
                print(data)
                
                start_id = data.find("*msgs*", i)
                if start_id == -1:
                    print("*msgs*")
                    return -1
                end_id = data.find("*msge*", start_id)
                if end_id == -1:
                    print("*msge*")
                    return -1
                
                end_work = data.find("*endw*", start_id, end_id)
                if end_work != -1:
                    print("*endw*")
                    return 0
                
                if data[start_id + 6: start_id + 12] == "*0000*":
                
                    path_id = data.find("*path*", start_id, end_id)
                    if path_id == -1:
                        print("*path*")
                        return -1
                    name_id = data.find("*name*", path_id, end_id)
                    if name_id == -1:
                        print("*name*")
                        return -1
                    i = end_id + 6
                    
                    path = data[path_id + 6: name_id]
                    name = data[name_id + 6: end_id]
                    sv.readANDfind_for_once(path, name)
                    
                    print("*************************************************")
                    print(path)
                    print(name)
                    print("*************************************************")
        

def main():
    HOST = "127.0.0.1"
    PORT = 2702
    start_work(HOST, PORT)
    


main()