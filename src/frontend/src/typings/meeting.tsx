export interface IMeetingSummary {
  title: string;
  id: string;
  time: string;
  place: string;
  summary: string;
  status: "obavljen" | "objavljen" | "planiran" | "arhiviran";
}
