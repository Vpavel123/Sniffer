class MockRawSocket : public RawSocket {
public:
    MOCK_METHOD(bool, checkSocket, (), (const override));
    MOCK_METHOD(int, recieve, (unsigned char* buffer, size_t lenght, int flag), (override));
    MOCK_METHOD(void, bindInterface, (const std::string& name), (override));
};