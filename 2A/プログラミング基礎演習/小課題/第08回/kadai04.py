def generate_short_url(url):
    digits = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    while True:
        hash = abs(url.__hash__())
        hash_string = ''
        while hash > 0:
            hash_string += digits[int(hash) % 62]
            hash = int(hash / 62)
        if not hash_string in url_dict.keys():
            url_dict[hash_string] = url
            break
    return hash_string

def get_long_url(hash):
    if hash_string in url_dict.keys():
        return url_dict[hash_string]
    else:
        return -1

url_dict = {}
hash_string = generate_short_url('http://hello.com')
long_url = get_long_url(hash_string)
if long_url == -1:
    print("No such url... \(-_-)/")
else:
    print("URL: " + long_url)
