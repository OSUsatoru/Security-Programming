# every information of equations are explained in my report.
# a littble bit inspired by this website to kickoff my class.
# https://siguniang.wordpress.com/2012/02/01/bloom-filter/

# https://pypi.org/project/mmh3/
import mmh3
import math

# https://pypi.org/project/bitarray/
from bitarray import bitarray



class BloomFilter(object):
    def __init__(self, dictionary_file, input_file):

        self.num_maybe = 0
        self.num_no = 0

        self.p = 0.0000001 # change this number to see a change.
        #self.p = 0.1
        #self.p = 0.5
        #self.p = 0.7

        self.n = self.get_dictionaly_size(dictionary_file)
        self.m = self.calc_bit_size()
        self.k = self.calc_num_hash()
        self.display_values()

        # create m bits array and set all 0
        self.bit_array = bitarray(self.m)
        self.bit_array.setall(0)

        self.process_dictionary_file(dictionary_file)

        self.process_input_file(input_file)

    # I need to get this number before I add words in bloom filter.
    def get_dictionaly_size(self, dictionary_file):
        return sum([1 for _ in open(dictionary_file,encoding="utf8", errors='ignore')])

    def calc_bit_size(self):
        return math.ceil((self.n * math.log(self.p)) / math.log(1 / math.pow(2, math.log(2))))

    def calc_num_hash(self):
        return round( (self.m/self.n) * math.log(2))

    def display_values(self):
        print("Number of words in dictionary: ", self.n)
        print("The size of bit array: ", self.m)
        print("Number of Hash functions: ", self.k)
        # https://techacademy.jp/magazine/23378 to display decimal number
        print("False Positive Rate: {:.07f}\n".format(self.p))

    # open file, to get each words to insert into bloom filter.
    def process_dictionary_file(self, dictionary_file):
        words = open(dictionary_file,encoding="utf8", errors='ignore').readlines()
        for word in words:
            word = word.replace("\n", "")
            self.insert_word(word)

        return

    # receive word and insert into each hash
    def insert_word(self, word):
        # insert word into each hash function
        for i in range(self.k):
            # generate hasg_value to get index of bit array
            hash_value = mmh3.hash(word, i) % self.m
            # mark this index
            self.bit_array[hash_value] = 1
        return

    def process_input_file(self, input_file):
        new_passwords = open(input_file).readlines()
        for password in new_passwords:
            password = password.replace("\n", "")
            self.bloom_filter(password)

        print("\nNumber of maybe: ", self.num_maybe)
        print("Number of no: ", self.num_no)

    def bloom_filter(self, password):
        is_checked = True

        for i in range(self.k):
            hash_value = mmh3.hash(password, i) % self.m
            # if there is 0, password is not in dictionary
            if self.bit_array[hash_value] == 0:
                is_checked = False

        if is_checked:
            print(password,": maybe")
            self.num_maybe+=1
        else:
            print(password,": no")
            self.num_no+=1
        return

def main():

    dictionary_file_name = "dictionary.txt"
    input_file_name = "sample_input.txt"

    BloomFilter(dictionary_file_name, input_file_name)

if __name__ == "__main__":
    main()