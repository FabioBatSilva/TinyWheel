import sys
import json

try:
    from dotenv import dotenv_values
except ModuleNotFoundError:
    print("module dotenv not found")
    print("try : pip install python-dotenv")
    sys.exit(1)

try:
    import requests
except ModuleNotFoundError:
    print("module requests not found")
    print("try : pip install requests")
    sys.exit(1)

PARAMETERS = dotenv_values()

assert PARAMETERS['WHEEL_DEVICE_API_KEY'], "WHEEL_DEVICE_NAME not defined"
assert PARAMETERS['WHEEL_DEVICE_TYPE'], "WHEEL_DEVICE_TYPE not defined"
assert PARAMETERS['WHEEL_DEVICE_NAME'], "WHEEL_DEVICE_API_KEY not defined"

WHEEL_DEVICE_NAME = PARAMETERS['WHEEL_DEVICE_NAME']
WHEEL_DEVICE_TYPE = PARAMETERS['WHEEL_DEVICE_TYPE']
WHEEL_DEVICE_API_KEY = PARAMETERS['WHEEL_DEVICE_API_KEY']
WHEEL_DEVICE_ID = PARAMETERS['WHEEL_DEVICE_NAME'].replace("ow", "")

assert int(WHEEL_DEVICE_ID) > 0, "Invalid WHEEL_DEVICE_ID : " + WHEEL_DEVICE_ID
assert WHEEL_DEVICE_TYPE in ['pint', 'xr'], "Invalid WHEEL_DEVICE_TYPE : " + WHEEL_DEVICE_TYPE
assert len(WHEEL_DEVICE_API_KEY) > 20, "Invalid WHEEL_DEVICE_API_KEY  : " + WHEEL_DEVICE_API_KEY

#REQUEST_BASE_URL = 'https://httpbin.org/anything'
REQUEST_BASE_URL = 'https://app.onewheel.com/wp-json/fm/v2/activation'
REQUEST_HEADERS = {
    'Accept' : 'application/json',
    'Content-Type' : 'application/json',
    'Connection' : 'keep-alive',
    'Accept-Language' : 'en-us',
    'Authorization' : 'Basic Og==',
    'Accept-Encoding' : 'gzip',
    'User-Agent' : 'Onewheel/0 CFNetwork/1121.2.2 Darwin/19.2.0',
    'Content-Type' : 'application/json'
}

print ("WHEEL_DEVICE_ID      : " + WHEEL_DEVICE_ID)
print ("WHEEL_DEVICE_NAME    : " + WHEEL_DEVICE_NAME)
print ("WHEEL_DEVICE_TYPE    : " + WHEEL_DEVICE_TYPE)
print ("WHEEL_DEVICE_API_KEY : " + WHEEL_DEVICE_API_KEY)
print ("REQUEST_BASE_URL     : " + REQUEST_BASE_URL)

answer = input('Are you sure : [y/n]')
if not answer or answer[0].lower() != 'y':
    print('Skipping..')
    exit(1)

response = requests.get(
    REQUEST_BASE_URL + "/" + WHEEL_DEVICE_ID,
    headers = REQUEST_HEADERS,
    params = {
        'owType': WHEEL_DEVICE_TYPE,
        'apiKey': WHEEL_DEVICE_API_KEY
    }
)

response_body = response.json()
response_code = response.status_code

print("")
print(response_code)
print(json.dumps(response_body, indent=4))

print ("WHEEL_DEVICE_NAME    : " + WHEEL_DEVICE_NAME)
print ("WHEEL_DEVICE_TYPE    : " + WHEEL_DEVICE_TYPE)
print ("WHEEL_DEVICE_API_KEY : " + WHEEL_DEVICE_API_KEY)
print ("WHEEL_DEVICE_KEY     : " + response_body['key'])

# RESPONSE
# HTTP/1.1 200 OK
# {
#   "key": "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
# }
