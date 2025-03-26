class Settings:
    '''
    the settings class. It reads from the settings.properties file and makes the contents available to any method
    that asks for them. It uses the Singleton principle
    '''
    __instance = None
    def __new__(cls, *args, **kwargs):
        if not cls.__instance:
            cls.__instance = super().__new__(cls)
        return cls.__instance

    def __init__(self):
        # check if the initialization took place already
        if hasattr(self, '_initialized'):
            return

        self._initialized = True

        f = open("./settings/settings.properties", "r")

        # read the input file
        x = f.readline()
        parts = x.split()
        self.__input = parts[2].strip("'")

        # read the output file
        x = f.readline()
        parts = x.split()
        self.__output = parts[2].strip("'")

    def get_input(self) -> str:
        '''
        returns the input file location as a string
        '''
        if self.__input == "":
            raise FileNotFoundError("No input file specified")
        return self.__input

    def get_output(self) -> str:
        '''
        returns the output file location as a string
        '''
        if self.__output == "":
            raise FileNotFoundError("No output file specified")
        return self.__output
