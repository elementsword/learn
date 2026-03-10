# rall

```cpp
#include <iostream>
#include <cstdio>
using namespace std;

class FileWrapper {
private:
    FILE* fp;   // 管理的资源

public:
    // 构造：获取资源
    FileWrapper(const char* filename, const char* mode) {
        fp = fopen(filename, mode);
        if (fp)
            cout << "文件打开成功\n";
        else
            cout << "文件打开失败\n";
    }

    // 禁止拷贝（因为两个对象同时 fclose 会出问题）
    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;

    // 析构：释放资源
    ~FileWrapper() {
        if (fp) {
            fclose(fp);
            cout << "文件已关闭\n";
        }
    }

    // 向文件写东西
    void write(const char* msg) {
        if (fp)
            fputs(msg, fp);
    }

    // 返回底层资源（如果你真的需要）
    FILE* get() const { return fp; }
};


int main() {
    {
        FileWrapper file("test.txt", "w");
        file.write("Hello RAII!\n");
    } 
    // 作用域结束，自动调用析构，文件关闭！

    cout << "作用域结束，文件自动关闭。\n";
}

```

