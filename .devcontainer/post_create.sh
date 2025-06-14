#!/bin/bash
# -----------------------------------------------------------------------------
# post_create.sh: postCreateCommand-command writing to $HOME/.bashrc
#
# Author: Bernhard Bablok
#
# -----------------------------------------------------------------------------

echo -e "[post_create.sh] starting postCreateCommand $0\n"
echo -e "[post_create.sh] PWD=$PWD\n"
echo -e "[post_create.sh] HOME=$HOME\n"

cat >> $HOME/.bashrc << "EOF"

if [ -f $HOME/install_build_env.log ]; then
  # setup already done
  echo "CircuitPython build-environment ready for $CP_TOOLCHAIN/$CP_PORT"
  echo "To start a build run:"
  echo "  cd ports/$CP_PORT"
  echo "  time make -j $(nproc) BOARD=your_board_name TRANSLATION=de_DE"
elif [ -f $HOME/install_build_env.log.active ]; then
  echo "Initial setup of build environment in progress, please wait."
  echo "Use 'tail -f $HOME/install_build_env.log.active' to monitor progress."
  echo "After successful installation, start a new terminal to build CircuitPython."
else
  echo "Starting initial setup of build environment, please wait"
  nohup /workspaces/circuitpython/.devcontainer/install_build_env.sh >> $HOME/nohup.out &
  echo "Use 'tail -f $HOME/install_build_env.log.active' to monitor progress."
  echo "After successful installation, start a new terminal to build CircuitPython."
fi

EOF
touch $HOME/post_create.finished

# --- that's it!   ------------------------------------------------------------

echo -e "[post_create.sh] setup complete\n"
