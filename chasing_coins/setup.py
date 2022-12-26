from setuptools import setup
from glob import glob
import os
package_name = 'chasing_coins'
share_list = ['launch','urdf','meshes','materials','worlds','models','config']
def trans(share_list):
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml'])]
    for name in share_list:
        cli = "(os.path.join('share', package_name, '{0}'), glob('{0}/**'))".format(name)
        if name == 'urdf':
            for urdf_name in ['chassis','sensors']:
                cli = "(os.path.join('share', package_name, '{0}/{1}'), glob('{0}/{1}/**'))".format(name,urdf_name)
                data_files.append(eval(cli))
            cli = "(os.path.join('share', package_name, '{0}'), glob('{0}/chasing_car.urdf.xacro'))".format(name)
            data_files.append(eval(cli))
            cli = "(os.path.join('share', package_name, '{0}'), glob('{0}/vehicle_config.yaml'))".format(name)
            data_files.append(eval(cli))
            continue
        if name == 'models':
            cli = "(os.path.join('share', package_name, '{0}/crazy_coins/meshes'), glob('{0}/crazy_coins/meshes/**'))".format(name)
            data_files.append(eval(cli))
            cli = "(os.path.join('share', package_name, '{0}/crazy_coins'), glob('{0}/crazy_coins/model.config'))".format(name)
            data_files.append(eval(cli))
            cli = "(os.path.join('share', package_name, '{0}/crazy_coins'), glob('{0}/crazy_coins/model.sdf'))".format(name)
            data_files.append(eval(cli))
            cli = "(os.path.join('share', package_name, '{0}/ground_plane'), glob('{0}/ground_plane/**'))".format(name)
            data_files.append(eval(cli))
            continue
        data_files.append(eval(cli))
    # for directory in glob("models/*"):
    #     data_file = (os.path.join(os.path.join('share',package_name), directory), glob(directory + "/*"))
    #     data_files.append(data_file)
    return data_files
setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=trans(share_list),
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='wily',
    maintainer_email='hitwhcar23@126.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
        ],
    },
)