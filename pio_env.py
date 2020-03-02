Import("env")

import os

def load_dotenv_params():
    params = {}

    if not os.path.isfile('.env'):
        return params

    with open('.env') as f:
        for item in f:
            line = item.strip()

            if not line or line.startswith('#'):
                continue

            key, value  = line.split('=', 1)
            params[key] = value

    return params


PARAMETERS = load_dotenv_params()
WHEEL_DEVICE_TYPE = PARAMETERS['WHEEL_DEVICE_TYPE']

assert WHEEL_DEVICE_TYPE, "WHEEL_DEVICE_TYPE not defined"

PARAMETERS.update({
    'APP' : os.getenv('APP', 'APP_MONITOR'),
    'WHEEL_DEVICE_TYPE' : 'WHEEL_DEVICE_TYPE_' + WHEEL_DEVICE_TYPE.upper()
})

for key, value in PARAMETERS.items():
    env.Append(CPPDEFINES=[(key, value)])
