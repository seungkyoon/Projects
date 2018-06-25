# Projects

To run the notebook, you will need to set up a Python environment with the appropriate dependencies. The following example will be ran using ``virtualenv``.

## Virtual Environment

Install the virtual environment, install required packages, and activate notebook extension

	sudo pip install virtualenv
	virtualenv ~/sitkpy --no-site-packages
	~/sitkpy/bin/pip install -r requirements.txt
	~/sitkpy/bin/jupyter nbextension enable --py --sys-prefix widgetsnbextension

## Run the notebook

To run the notebook:

	~/sitkpy/bin/jupyter notebook