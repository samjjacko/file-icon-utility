class OSIconInterface 
{
    public:
        OSIconInterface();
        ~OSIconInterface();
        bool setIconWithLocalImg(char* target_path, char* img_path);
        int setIconWithURL(char* target_path, char* img_url);
        const char* fetchDtopImgURL();
    private:
        struct Impl; 
        Impl* impl; // will point to an instance of the OS specific interface
        // void customInit();
};
