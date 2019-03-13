let component = React.statelessComponent("IconHeart");

let make = (~className="", _children) => {
  ...component,
  render: _self =>
    <svg className={"icon ion-ios-heart " ++ className} viewBox="0 0 512 512">
      <path
        d="M352,56C352,56,352,56,352,56C352,56,352,56,352,56c-0.3,0-0.7,0-1,0c-39.7,0-74.8,21-95,52c-20.2-31-55.3-52-95-52
	c-0.3,0-0.7,0-1,0c0,0,0,0,0,0c0,0,0,0,0,0C98.1,56.6,48,106.9,48,169c0,37,16.2,89.5,47.8,132.7C156,384,256,456,256,456
	s100-72,160.2-154.3C447.8,258.5,464,206,464,169C464,106.9,413.9,56.6,352,56z"
      />
    </svg>,
};