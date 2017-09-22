type joke = {
  icon_url: string,
  value: string,
};

let parseChuckJokeJson json :joke =>
  Json.Decode.{
    icon_url: field "icon_url" string json,
    value: field "value" string json
  };

let chuckApi = "https://api.chucknorris.io/jokes/random?category=dev";

let fetchJoke _name =>
  Bs_fetch.fetch chuckApi
    |> Js.Promise.then_ Bs_fetch.Response.json
    |> Js.Promise.then_ (fun json =>
    Js.Promise.resolve (Js.log (parseChuckJokeJson json ))
);
  
module G = {
  type action =
    | Tick
    | UpdateClick;

  type state = {
    count: int,
    timerId: ref (option Js.Global.intervalId)
  };

  let component = ReasonReact.reducerComponent "Counter";

  let make _children => {
    ...component,
    initialState: fun () => {count: 0, timerId: ref None},
    reducer: fun action state =>
      switch action {
      | Tick => ReasonReact.Update {...state, count: state.count + 1}
      | UpdateClick => ReasonReact.SideEffects (fun _self => {
        Js.log "hello!";
        Js.log "goodbye!";
        fetchJoke "hi" |> ignore;
        /*
        Js.Promise.(
          Bs_fetch.fetch "/api/hellos/1"
          |> then_ Bs_fetch.Response.text
          |> then_ (fun text => print_endline text |> Js.Promise.resolve)
        );
        */
      })
      },
    didMount: fun self => {
      Js.log "Hello~";
      self.state.timerId := Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) 1000);
      ReasonReact.NoUpdate
    },
    render: fun self => {
      <div 
        onClick=(self.reduce (fun _event => UpdateClick))> 
        (ReasonReact.stringToElement (string_of_int self.state.count)) 
      </div>
    }
  };
};
ReactDOMRe.renderToElementWithId <G /> "index";
/* ReactDOMRe.renderToElementWithClassName <Top /> "todoapp"; */
