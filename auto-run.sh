systemctl daemon-reload
systemctl stop nerf_geometry
SERVICE=/etc/systemd/system/nerf_geometry.service
rm -rf $SERVICE
CUR_DIR=$(cd `dirname $0` && pwd -P)
echo "[Unit]" > $SERVICE
echo "Description=Nerf-demo" >> $SERVICE
echo "" >> $SERVICE
echo "[Service]" >> $SERVICE
echo "Type=simple" >> $SERVICE
echo "Environment=" >> $SERVICE
echo "ExecStart=${CUR_DIR}/bin/SampleGeometryDetect" >> $SERVICE
echo "Restart=always" >> $SERVICE
echo "RestartSec=1" >> $SERVICE
echo "[Install]" >> $SERVICE
echo "WantedBy=multi-user.target" >> $SERVICE
systemctl start nerf_geometry
