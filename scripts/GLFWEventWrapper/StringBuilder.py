class StringBuilder:
    def __init__(self, startString):
        self.String = startString

    def Append(self, string):
        self.String += string
    
    def AppendLine(self, string):
        self.String += string + '\n'

    def GetString(self):
        return self.String