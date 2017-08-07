load(":Linux/api.bzl", "zq_linux_driver", 'zq_linux_config')
load("//BuildTools:config.bzl", "config")

platform = config['platform']

functions = {
    'Linux': {
        'zq_driver': zq_linux_driver,
        'zq_config': zq_linux_config,
    },
}

zq_config = functions[platform]['zq_config']
zq_driver = functions[platform]['zq_driver']

#def zq_driver (**args):
    #f = functions[platform]['zq_driver']
    #f(**args)
