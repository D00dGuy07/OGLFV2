from SectionBuilder import GetHeader, GetSource
from EventConfig import GetConfig

def main():
    config = GetConfig()

    headerText = GetHeader(config)
    with open("GLFWEventWrapper.h", "w") as file:
        file.write(headerText)

    sourceText = GetSource(config)
    with open("GLFWEventWrapper.cpp", "w") as file:
        file.write(sourceText)

main()