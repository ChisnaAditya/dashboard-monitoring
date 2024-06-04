import { database } from "../firebase";
import { onValue, ref } from "firebase/database";
import { useEffect, useState } from "react";
import DataTable from "react-data-table-component";

export default function TableComponent() {
  const [reading, setReading] = useState("");

  const columns = [
    {
      name: "Date & Time",
      selector: (row) => row.tanggal,
      sortable: true,
    },
    {
      name: "Suhu",
      selector: (row) => row.suhu,
    },
    {
      name: "DO",
      selector: (row) => row.oksigen,
    },
    {
      name: "PH",
      selector: (row) => row.ph,
    },
    {
      name: "Relay",
      selector: (row) => row.relay,
    },
  ];

  const db = database;
  const path = ref(db, "database");

  const convertToDateTime = (timeepoc) => {
    return new Date(timeepoc * 1000);
  };

  useEffect(() => {
    onValue(path, (snapshot) => {
      const data = snapshot.val();

      let a = Object.values(data.reading.timestamp);
      let b = Object.values(data.reading.SUHU);
      let c = Object.values(data.reading.DO);
      let d = Object.values(data.reading.PH);
      let e = Object.values(data.reading.RELAY);
      let f = Object.values(data.reading.timestamp);

      let obj = {};

      a.forEach(
        (k, i) =>
          (obj[k] = {
            id: k,
            tanggal: convertToDateTime(f[i]).toString(),
            suhu: b[i],
            oksigen: c[i],
            ph: d[i],
            relay: e[i] === 1 ? "ON" : "OFF",
          })
      );
      setReading(Object.values(Object.values(obj)));
    });
  }, []);

  return (
    <div className="bg-gradient-to-r rounded-3xl from-indigo-600 to-indigo-500">
      <div className="p-10">
        <DataTable
          columns={columns}
          data={Object.values(reading)}
          fixedHeader
          pagination
        />
      </div>
    </div>
  );
}
