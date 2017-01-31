# Home metrics

## Basic Architecture

```
NODE (Particle Proton w/ sensors)
 
 |
 v

Telegraf

 |
 v

InfluxDB

 |
 v

Grafana
```

## Stack

- [Particle](https://www.particle.io/) - could be replaced by anything, like ESP32*
- [Telegraf](https://docs.influxdata.com/telegraf/)
- [InfluxDB](https://docs.influxdata.com/influxdb/)
- [Grafana](http://grafana.org/)
