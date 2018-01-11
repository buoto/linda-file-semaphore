import unittest, os
from subprocess import Popen, PIPE, STDOUT

PROGRAM = './linda'
FILE = 'file.linda'

class LindaTests(unittest.TestCase):

    def tearDown(self):
        try:
            os.remove(FILE)
        except OSError:
            pass

    def output_run(self, table):
        for t in table:
            program = Popen([PROGRAM],stdin=PIPE, stdout=PIPE, stderr=PIPE)
            program.communicate(t[0].encode('utf-8'))

            with open(FILE) as f:
                line = f.readline()
                self.assertEqual(line, t[1])

    def read_run(self, table):
        for t in table:
            with open(FILE, 'w') as f:
                f.write(t[0])

            program = Popen([PROGRAM],stdin=PIPE, stdout=PIPE, stderr=PIPE)
            line = program.communicate(t[1].encode('utf-8'))[0]
            self.assertEqual(line, t[0].encode('utf-8'))

    def input_run(self, table):
        for t in table:
            with open(FILE, 'w') as f:
                f.write(t[0])

            program = Popen([PROGRAM],stdin=PIPE, stdout=PIPE, stderr=PIPE)
            line = program.communicate(t[1].encode('utf-8'))[0]
            self.assertEqual(line, t[0].encode('utf-8'))

            with open(FILE) as f:
                rline = f.readline()
                self.assertEqual(rline, '\n')

    def test_output(self):
        table = [['output(123, "asd", "hehe")', '(123,"asd","hehe")\n'],
                 ['output("the tuple")', '("the tuple")\n',]]
        self.output_run(table)

    def test_read(self):
        table = [['read("A*D", >=123)', '("ABCD", 200)\n'],
                 ['read(>1, "abc")', '(444, "abc")\n']]
        self.read_run(table)

    def test_input(self):
        table = [['read("A*D", >=123)', '("ABCD", 200)\n'],
                 ['read(>1, "abc")', '(444, "abc")\n']]
        self.input_run(table)

if __name__ == '__main__':
    unittest.main(warnings=None)
